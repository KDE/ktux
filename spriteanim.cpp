//---------------------------------------------------------------------------
//
// spriteanim.cpp
//
// Copyright (c) 1999 Martin R. Jones <mjones@kde.org>
//

#include "spritemisc.h"
#include "spritepm.h"
#include "spriteanim.h"
#include <kdebug.h>

//===========================================================================
//
// SpriteObject stores the animations that create an object
//
SpriteObject::SpriteObject(SpritePixmapSequence *seq, QCanvas *c )
    : QCanvasSprite(seq, c),
      mCycle(0),
      mLifeSpan(-1),
      mSeq(seq)
{
}

//---------------------------------------------------------------------------
void SpriteObject::age()
{
    if (mLifeSpan > 0)
    {
        mLifeSpan--;
    }
    mCycle++;
    if (mCycle > mSeq->delay(frame()))
    {
        setFrame((frame()+1)%frameCount());
        mCycle = 0;
    }
}

void SpriteObject::setBounds( int x1, int y1, int x2, int y2 )
{
    mBound = QRect( x1, y1, x2-x1, y2-y1 );
}

bool SpriteObject::outOfBounds() const
{
    bool in = mBound.contains( static_cast<int>(x()), static_cast<int>(y()) );
    return !in;
}

//===========================================================================
//
// SpriteDef stores the animations that create an object
//
SpriteDef::SpriteDef(KConfigBase &config)
{
    read(config);
}

//---------------------------------------------------------------------------
SpriteObject *SpriteDef::create( QCanvas *c )
{
    SpriteObject *sprite = 0;
    if (mSeq)
    {
        int startX = mStartX.random();
        int startY = mStartY.random();
        sprite = new SpriteObject(mSeq, c);
        sprite->setVelocity(mDirX.random(), mDirY.random());
	if ( mDirX.min() != 0 || mDirX.max() != 0 ||
	     mDirY.min() != 0 || mDirY.max() != 0 ) {
	    sprite->setAnimated( true );
	}
        sprite->move(startX, startY);
        sprite->setBounds(startX-1, startY-1, mEndX.random()+1, mEndY.random()+1);
        sprite->setLifeSpan(mLifeSpan);
        sprite->setZ(mZ);
        sprite->show();
    }

    return sprite;
}

//---------------------------------------------------------------------------
void SpriteDef::read(KConfigBase &config)
{
    mDirX.set(config.readEntry("DirectionX", "0"));
    mDirY.set(config.readEntry("DirectionY", "0"));
    mStartX.set(config.readEntry("StartX", "0"));
    mStartY.set(config.readEntry("StartY", "0"));
    mEndX.set(config.readEntry("EndX", "10000"));
    mEndY.set(config.readEntry("EndY", "10000"));
    mLifeSpan = config.readNumEntry("LifeSpan", -1);
    mZ = config.readNumEntry("Z", 1);
    QString animation = config.readEntry("Animation", "");
    mSeq = SpriteSequenceManager::manager()->load(config, animation);
    kdDebug() << "Set Z = " << mZ << endl;
}

//===========================================================================
//
// SpriteGroup
//
SpriteGroup::SpriteGroup(QCanvas *c, KConfigBase &config)
    : mCanvas(c)
{
    mAvailable.setAutoDelete(true);
    mActive.setAutoDelete(true);
    read(config);
}

//---------------------------------------------------------------------------
void SpriteGroup::next()
{
    QPtrListIterator<SpriteObject> it(mActive);

    for (; it.current(); ++it)
    {
        SpriteObject *sprite = it.current();
        if (sprite->outOfBounds() || sprite->dead())
        {
            mActive.removeRef(sprite);
        }
        else
        {
//            sprite->forward(1);
            sprite->age();
        }
    }
}

//---------------------------------------------------------------------------
void SpriteGroup::refresh()
{
    if (((int) mActive.count()) < mCount)
    {
        SpriteObject *sprite = mAvailable.first()->create(mCanvas);
        mActive.append(sprite);
    }
}


//---------------------------------------------------------------------------
void SpriteGroup::read(KConfigBase &config)
{
    SpriteRange countRange(config.readEntry("Count", "1"));
    mCount = countRange.random();

    mRefresh.set(config.readEntry("Refresh", "1000"));

    QStrList anims;
    config.readListEntry("Animations", anims);

    for (anims.first(); anims.current(); anims.next())
    {
        config.setGroup(anims.current());
        SpriteDef *obj = new SpriteDef(config);
        mAvailable.append(obj);
    }
}

