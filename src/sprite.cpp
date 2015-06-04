/*
 *   Copyright 1999 by Martin R. Jones <mjones@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "sprite.h"
#include "spritepm.h"
#include "spritemisc.h"

#include <cstdlib>
#include <ctime>

#include <QApplication>
#include <QDebug>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPixmap>
#include <QPointer>
#include <QSlider>
#include <QStandardPaths>
#include <QTimerEvent>
#include <QVBoxLayout>

#include <KConfigGroup>
#include <KSharedConfig>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KSpriteSaver topLevel;
    topLevel.setWindowTitle( QLatin1String( "KTux" ));

    return app.exec();
}

KSpriteSaver::KSpriteSaver()
{
    initialise();
    readSettings();
    blank();

    connect(&mTimer, SIGNAL(timeout()), SLOT(slotTimeout()));
    mTimer.setSingleShot(true);
    mTimer.start(120-mSpeed);
}


KSpriteSaver::~KSpriteSaver()
{
    mTimer.stop();
    delete mView;
    delete mScene;
}


void KSpriteSaver::setSpeed(int speed)
{
    mSpeed = speed;
    mTimer.start(120-mSpeed);
}


void KSpriteSaver::readSettings()
{
    QString str;

    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup group( config, "Settings" );

    mSpeed = group.readEntry("Speed", 50);

    QString path = QStandardPaths::locate(QStandardPaths::DataLocation, QStringLiteral("sprites/"), QStandardPaths::LocateDirectory);

    SpritePixmapManager::manager()->setPixmapDir(path);

    path += QLatin1String( "spriterc" );

    KConfig *pConfig = new KConfig(path);
    KConfigGroup mConfig(pConfig, "Config");
    QStringList list;
    list = mConfig.readEntry("Groups",list);
    mTimerIds.resize(list.count());
    for (int i = 0; i < list.count(); i++)
    {
        qDebug() << "Group: " << list.at(i);
        KConfigGroup grp = pConfig->group(list.at(i));
        SpriteGroup *obj = new SpriteGroup(mScene, grp);
        mTimerIds[i] = startTimer(obj->refreshTime());
        mGroups.append(obj);
    }
    delete pConfig;
}


void KSpriteSaver::initialise()
{
    mScene = new QGraphicsScene();
    QPixmap pm( QStandardPaths::locate(QStandardPaths::DataLocation, QStringLiteral("sprites/bg.png")) );
    mScene->setBackgroundBrush(QBrush(pm));
    mScene->setSceneRect( 0, 0, width(), height() );
    mView = new QGraphicsView(mScene);

    QPalette palette;
    palette.setColor(mView->viewport()->backgroundRole(), Qt::black);
    mView->viewport()->setPalette(palette);

    mView->resize( size());
    mView->setFrameStyle( QFrame::NoFrame );
    mView->show();
    SpriteRange::setFieldSize(mView->size());
}


void KSpriteSaver::slotTimeout()
{
    mTimer.setSingleShot(true);
    mTimer.start(120-mSpeed);
    SpriteGroup *grp;

    Q_FOREACH( grp, mGroups )
    {
        grp->next();
    }

    mScene->advance();
}


void KSpriteSaver::timerEvent(QTimerEvent *ev)
{
    for (int i = 0; i < mTimerIds.size(); i++)
    {
        if (mTimerIds[i] == ev->timerId())
        {
            mGroups.at(i)->refresh();
            killTimer(ev->timerId());
            mTimerIds[i] = startTimer(mGroups.at(i)->refreshTime());
            break;
        }
    }
}


void KSpriteSaver::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED( event );
    mScene->setSceneRect( 0, 0, width(), height() );
}


void KSpriteSaver::blank()
{
    QPalette palette;
    palette.setColor(this->backgroundRole(), Qt::black);
    this->setPalette(palette);

    update();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
