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
#include "spriteanim.h"
#include "spritemisc.h"
#include "spritepm.h"

#include <kdebug.h>
#include <kconfiggroup.h>


SpriteObject::SpriteObject(SpritePixmapSequence *seq, Q3Canvas *c)
  : Q3CanvasSprite( seq, c ),
    mCycle( 0 ),
    mLifeSpan( -1 ),
    mSeq( seq )
{
}


void SpriteObject::age()
{
    if( mLifeSpan > 0 ) {
        mLifeSpan--;
    }
    mCycle++;

    if( mCycle > mSeq->delay( frame() ) ) {
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
    return !mBound.contains( static_cast<int>( x() ), static_cast<int>( y() ) );
}



SpriteDef::SpriteDef(KConfigGroup &config)
{
    read( config );
}


SpriteObject *SpriteDef::create( Q3Canvas *c )
{
    SpriteObject *sprite = 0;
    if( mSeq ) {
        int startX = mStartX.random();
        int startY = mStartY.random();
        sprite = new SpriteObject( mSeq, c );
        sprite->setVelocity( mDirX.random(), mDirY.random() );
        if( mDirX.min() != 0 || mDirX.max() != 0 ||
            mDirY.min() != 0 || mDirY.max() != 0 ) {
            sprite->setAnimated( true );
        }
        sprite->move( startX, startY );
        sprite->setBounds( startX-1, startY-1, mEndX.random()+1, mEndY.random()+1 );
        sprite->setLifeSpan( mLifeSpan );
        sprite->setZ( mZ );
        sprite->show();
    }

    return sprite;
}


void SpriteDef::read(KConfigGroup &config)
{
    mDirX.set( config.readEntry( "DirectionX", "0" ) );
    mDirY.set( config.readEntry( "DirectionY", "0" ) );
    mStartX.set( config.readEntry( "StartX", "0" ) );
    mStartY.set( config.readEntry( "StartY", "0" ) );
    mEndX.set( config.readEntry( "EndX", "10000" ) );
    mEndY.set( config.readEntry( "EndY", "10000" ) );
    mLifeSpan = config.readEntry( "LifeSpan", -1 );
    mZ = config.readEntry( "Z", 1 );
    QString animation = config.readEntry( "Animation", "" );
    KConfigBase *grp = config.config();
    mSeq = SpriteSequenceManager::manager()->load( *grp, animation );
    kDebug() << "Set Z = " << mZ;
}



SpriteGroup::SpriteGroup(Q3Canvas *c, KConfigGroup &config)
  : mCanvas( c )
{
    mAvailable.setAutoDelete( true );
    mActive.setAutoDelete( true );
    read( config );
}


void SpriteGroup::next()
{
    Q3PtrListIterator<SpriteObject> it(mActive);

    for(; it.current(); ++it) {
        SpriteObject *sprite = it.current();
        if( sprite->outOfBounds() || sprite->dead() ) {
            mActive.removeRef( sprite );
        }
        else {
//            sprite->forward(1);
            sprite->age();
        }
    }
}


void SpriteGroup::refresh()
{
    if( ((int) mActive.count()) < mCount ) {
        SpriteObject *sprite = mAvailable.first()->create( mCanvas );
        mActive.append( sprite );
    }
}


void SpriteGroup::read(KConfigGroup &config)
{
    qDebug() << " void SpriteGroup::read(KConfigBase &config) :" << config.name();

    SpriteRange countRange( config.readEntry( "Count", "1" ) );
    mCount = countRange.random();

    mRefresh.set( config.readEntry( "Refresh", "1000" ) );

    QStringList anims;
    anims = config.readEntry( "Animations", anims );
    QStringList::const_iterator lst;

    for( lst = anims.constBegin(); lst != anims.constEnd(); ++lst ) {
        KConfigGroup grp( config.config(), *lst );
        SpriteDef *obj = new SpriteDef( grp );
        mAvailable.append( obj );
    }
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1: