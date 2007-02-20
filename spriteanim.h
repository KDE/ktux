//---------------------------------------------------------------------------
//
// spriteanim.h
//
// Copyright (c) 1999 Martin R. Jones <mjones@kde.org>
//

#ifndef SPRITEANIM_H
#define SPRITEANIM_H

#include <qpixmap.h>
#include <q3dict.h>
#include <q3ptrlist.h>
#include <q3strlist.h>
#include <q3canvas.h>
#include <kconfigbase.h>
#include <kconfig.h>

//---------------------------------------------------------------------------
//
// SpriteObject stores the animations that create an object
//
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
    int                     mCycle;
    int                     mLifeSpan;
    SpritePixmapSequence    *mSeq;
    QRect		    mBound;
};

//---------------------------------------------------------------------------
//
// SpriteDef stores the definition of a sprite
//
class SpriteDef
{
public:
    explicit SpriteDef(KConfigBase &config);

    SpriteObject *create( Q3Canvas *c );

protected:
    void read(KConfigBase &config);

protected:
    SpriteRange             mDirX;
    SpriteRange             mDirY;
    SpriteRange             mStartX;
    SpriteRange             mStartY;
    SpriteRange             mEndX;
    SpriteRange             mEndY;
    int                     mLifeSpan;
    int                     mZ;
    SpritePixmapSequence    *mSeq;
};

//---------------------------------------------------------------------------
//
// SpriteGroup
//
class SpriteGroup
{
public:
    SpriteGroup(Q3Canvas *c, KConfigBase &config);

    void next();
    void refresh();
    int refreshTime() const { return mRefresh.random(); }

protected:
    void read(KConfigBase &config);

protected:
    Q3PtrList<SpriteDef>        mAvailable;
    Q3PtrList<SpriteObject>     mActive;
    int                     mCount;
    SpriteRange             mRefresh;
    Q3Canvas		    *mCanvas;
};

#endif // SPRITEANIM_H

