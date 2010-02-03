/*
 *   Copyright 1996 by Martin R. Jones <mjones@kde.org>
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
#ifndef SPRITE_H
#define SPRITE_H

#include <QTimer>
#include <QPushButton>
#include <QTimerEvent>
#include <QVector>

#include <Qt3Support/Q3Canvas>

#include <kscreensaver.h>
#include <KDialog>

#include "spriteanim.h"


class KSpriteSaver : public KScreenSaver
{
    Q_OBJECT

    public:
        explicit KSpriteSaver(WId id = 0);
        virtual ~KSpriteSaver();

    public slots:
        void setSpeed(int speed);

    private:
        void readSettings();
        void initialise();
        void blank();

    protected slots:
        void slotTimeout();

    protected:
        virtual void timerEvent(QTimerEvent *event);

    protected:
        Q3Canvas *mCanvas;
        Q3CanvasView *mView;
        QTimer mTimer;
        int mSpeed;
        QList<SpriteGroup*> mGroups;
        QVector<int> mTimerIds;
};


class KSpriteSetup : public KDialog
{
    Q_OBJECT

    public:
        explicit KSpriteSetup(QWidget *parent = 0, const char *name = 0);
        ~KSpriteSetup();

    protected:
        void readSettings();

    private slots:
        void slotSpeed(int s);
        void slotOkPressed();
        void slotAbout();

    private:
        int speed;
        QWidget *preview;
        KSpriteSaver *saver;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
