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
#include "spritepm.h"

#include <cstdlib>
#include <KConfigGroup>

#include <QDebug>
#include <QPixmap>
#include <QVector>


// static
SpritePixmapManager *SpritePixmapManager::mManager = 0;


SpritePixmapManager::SpritePixmapManager()
  : mPixmapDir(QLatin1String("."))
{
}


SpritePixmapManager::~SpritePixmapManager()
{
}


QPixmap *SpritePixmapManager::load(const QString & img)
{
    QHash<QString, QPixmap*>::iterator i = mPixmaps.find( img );
    QPixmap *pixmap = i.value();

    if( i == mPixmaps.end() ) {
        // pixmap has not yet been loaded.
        qDebug() << "Reading pixmap: " << img;
        QString path = mPixmapDir + QLatin1String("/") + img;
        pixmap = new QPixmap(path);

        if( !pixmap->isNull() ) {
            mPixmaps.insert(img,pixmap);
        }
        else {
            delete pixmap;
            pixmap = 0;
            qDebug() << "read failed";
        }
    }

    return pixmap;
}


SpritePixmapManager *SpritePixmapManager::manager()
{
    if( !mManager ) {
        mManager = new SpritePixmapManager();
    }

    return mManager;
}


SpritePixmapSequence::SpritePixmapSequence(QList<QPixmap*> pm, QList<QPoint*> hs, QVector<int> d)
    : mDelays( d ), pm(pm), hs(hs)
{
}

SpriteSequenceManager *SpriteSequenceManager::mManager = 0;



SpriteSequenceManager::SpriteSequenceManager()
{
}


SpriteSequenceManager::~SpriteSequenceManager()
{
}


SpritePixmapSequence* SpriteSequenceManager::load(KConfigBase &config, const QString &name)
{
    QHash<QString, SpritePixmapSequence*>::iterator i = mSprites.find( name );
    SpritePixmapSequence *sprite = i.value();

    if( i == mSprites.end() ) {
        qDebug() << "Reading sprite: " << name;
        KConfigGroup grp( &config, name );
        sprite = read( grp );
        if( sprite ) {
            mSprites.insert(name, sprite);
        }
    }

    return sprite;
}


SpritePixmapSequence *SpriteSequenceManager::read(const KConfigGroup &config)
{
    QStringList strImages;
    QStringList strDelays;
    QList<QPixmap*> pixmaps;
    QList<QPoint*> hotspots;

    strImages = config.readEntry( "Images",QStringList() );
    strDelays = config.readEntry( "Delays",QStringList() );

    QVector<int> delays( strImages.count() );

    for( int i = 0; i < strImages.count(); i++ ) {
        QPixmap *pixmap = SpritePixmapManager::manager()->load( strImages.at( i ) );
        if( pixmap ) {
            pixmaps.append( pixmap );
            hotspots.append( new QPoint( 0,0 ) );
            delays[i] = strDelays.at( i ).toInt();
        }
    }

    return new SpritePixmapSequence( pixmaps, hotspots, delays );
}


SpriteSequenceManager *SpriteSequenceManager::manager()
{
    if( !mManager ) {
        mManager = new SpriteSequenceManager();
    }

    return mManager;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
