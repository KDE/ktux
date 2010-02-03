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
#ifndef SPRITEANIM_H
#define SPRITEANIM_H

#include "spritemisc.h"
#include "spritepm.h"

#include <QPixmap>

#include <Qt3Support/Q3Dict>
#include <Qt3Support/Q3PtrList>
#include <Qt3Support/Q3StrList>
#include <Qt3Support/Q3Canvas>

#include <kconfigbase.h>
#include <kconfig.h>



class SpriteObject : public Q3CanvasSprite
{
    public:
        SpriteObject(SpritePixmapSequence *seq, Q3Canvas *c);

        void setLifeSpan(int l) { mLifeSpan = l; }
        void age();
        bool dead() const { return (mLifeSpan == 0); }
        void setBounds( int x1, int y1, int x2, int y2 );
        bool outOfBounds() const;

    protected:
        int mCycle;
        int mLifeSpan;
        SpritePixmapSequence *mSeq;
        QRect mBound;
};


class SpriteDef
{
    public:
        explicit SpriteDef(KConfigGroup &config);

        SpriteObject *create(Q3Canvas *c);

    protected:
        void read(KConfigGroup &config);

    protected:
        SpriteRange mDirX;
        SpriteRange mDirY;
        SpriteRange mStartX;
        SpriteRange mStartY;
        SpriteRange mEndX;
        SpriteRange mEndY;
        int mLifeSpan;
        int mZ;
        SpritePixmapSequence *mSeq;
};


class SpriteGroup
{
    public:
        SpriteGroup(Q3Canvas *c, KConfigGroup &config);

        void next();
        void refresh();
        int refreshTime() const { return mRefresh.random(); }

    protected:
        void read(KConfigGroup &config);

    protected:
        Q3PtrList<SpriteDef> mAvailable;
        Q3PtrList<SpriteObject> mActive;
        int mCount;
        SpriteRange mRefresh;
        Q3Canvas *mCanvas;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
