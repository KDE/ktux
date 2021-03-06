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

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QTimerEvent>
#include <QVector>

#include "spriteanim.h"


class KSpriteSaver : public QMainWindow
{
    Q_OBJECT

    public:
        explicit KSpriteSaver();
        virtual ~KSpriteSaver();

    public Q_SLOTS:
        void setSpeed(int speed);

    private:
        void readSettings();
        void initialise();
        void blank();

    protected Q_SLOTS:
        void slotTimeout();

    protected:
        void timerEvent(QTimerEvent *event);
        void resizeEvent(QResizeEvent *event);

    protected:
        QGraphicsScene *mScene;
        QGraphicsView *mView;
        QTimer mTimer;
        int mSpeed;
        QList<SpriteGroup*> mGroups;
        QVector<int> mTimerIds;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
