//---------------------------------------------------------------------------
//
// spriteanim.h
//
// Copyright (c) 1999 Martin R. Jones <mjones@kde.org>
//

#ifndef SPRITEANIM_H
#define SPRITEANIM_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <qpixmap.h>
#include <qdict.h>                                                              
#include <qptrlist.h>
#include <qstrlist.h>
#include <qcanvas.h>
#include <kconfigbase.h>
#include <ksimpleconfig.h>                                                      

//---------------------------------------------------------------------------
//
// SpriteObject stores the animations that create an object
//
class SpriteObject : public QCanvasSprite
{
public:
    SpriteObject(SpritePixmapSequence *seq, QCanvas *c);

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
    SpriteDef(KConfigBase &config);

    SpriteObject *create( QCanvas *c );

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
    SpriteGroup(QCanvas *c, KConfigBase &config);

    void next();
    void refresh();
    int refreshTime() const { return mRefresh.random(); }

protected:
    void read(KConfigBase &config);

protected:
    QPtrList<SpriteDef>        mAvailable;
    QPtrList<SpriteObject>     mActive;
    int                     mCount;
    SpriteRange             mRefresh;
    QCanvas		    *mCanvas;
};

#endif // SPRITEANIM_H

