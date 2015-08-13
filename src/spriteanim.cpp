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

#include <QDebug>
#include <QPainter>

#include <KConfigGroup>


SpriteObject::SpriteObject(SpritePixmapSequence *seq, QGraphicsScene *scene)
  : QGraphicsItem(0),
    mCycle( 0 ),
    mLifeSpan( -1 ),
    mSeq( seq ),
    currentFrame( 0 )
{
    for (int i = 0; i < mSeq->pixmaps().size(); ++i) {
        QPixmap *pixmap;
        pixmap = mSeq->pixmaps().at(i);
        Frame frame;
        frame.pixmap = pixmap;
        frame.shape = QPainterPath();
        frame.boundingRect = pixmap->rect();
        frames << frame;
    }

    scene->addItem(this);
}

void SpriteObject::setFrame(int frame)
{
    if (!frames.isEmpty()) {
        prepareGeometryChange();
        currentFrame = frame % frames.size();
    }
}

void SpriteObject::age()
{
    if( mLifeSpan > 0 ) {
        mLifeSpan--;
    }
    mCycle++;
    moveBy(vx, vy);

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


SpriteObject *SpriteDef::create( QGraphicsScene *scene )
{
    SpriteObject *sprite = 0;
    if( mSeq ) {
        int startX = mStartX.random();
        int startY = mStartY.random();
        sprite = new SpriteObject( mSeq, scene );
        sprite->setVelocity( mDirX.random(), mDirY.random() );
        if( mDirX.min() != 0 || mDirX.max() != 0 ||
            mDirY.min() != 0 || mDirY.max() != 0 ) {
        }
        sprite->setPos( startX, startY );
        sprite->setBounds( startX-1, startY-1, mEndX.random()+1, mEndY.random()+1 );
        sprite->setLifeSpan( mLifeSpan );
        sprite->setZValue( mZ );
        sprite->show();
    }

    return sprite;
}


void SpriteDef::read(KConfigGroup &config)
{
    mDirX.set( config.readEntry( QStringLiteral("DirectionX"), QStringLiteral("0") ) );
    mDirY.set( config.readEntry( QStringLiteral("DirectionY"), QStringLiteral("0") ) );
    mStartX.set( config.readEntry( QStringLiteral("StartX"), QStringLiteral("0") ) );
    mStartY.set( config.readEntry( QStringLiteral("StartY"), QStringLiteral("0") ) );
    mEndX.set( config.readEntry( QStringLiteral("EndX"), QStringLiteral("10000") ) );
    mEndY.set( config.readEntry( QStringLiteral("EndY"), QStringLiteral("10000") ) );
    mLifeSpan = config.readEntry( QStringLiteral("LifeSpan"), -1 );
    mZ = config.readEntry( QStringLiteral("Z"), 1 );
    QString animation = config.readEntry( QStringLiteral("Animation"), QString() );
    KConfigBase *grp = config.config();
    mSeq = SpriteSequenceManager::manager()->load( *grp, animation );
    qDebug() << QStringLiteral("Set Z = ") << mZ;
}



SpriteGroup::SpriteGroup(QGraphicsScene *scene, KConfigGroup &config)
  : mScene( scene )
{
    read( config );
}


void SpriteGroup::next()
{
    QList<SpriteObject*>::iterator it;

    for(it = mActive.begin(); it != mActive.end(); ++it) {
        SpriteObject *sprite = *it;
        if( sprite->outOfBounds() || sprite->dead() ) {
            int i = mActive.indexOf(sprite);
            if (i != -1)
            {
                delete mActive.takeAt(i);
            }
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
        SpriteObject *sprite = mAvailable.first()->create( mScene );
        mActive.append( sprite );
    }
}


void SpriteGroup::read(KConfigGroup &config)
{
    qDebug() << QStringLiteral(" void SpriteGroup::read(KConfigBase &config) :") << config.name();

    SpriteRange countRange( config.readEntry( QStringLiteral("Count"), QStringLiteral("1") ) );
    mCount = countRange.random();

    mRefresh.set( config.readEntry( QStringLiteral("Refresh"), QStringLiteral("1000") ) );

    QStringList anims;
    anims = config.readEntry( QStringLiteral("Animations"), anims );
    QStringList::const_iterator lst;

    for( lst = anims.constBegin(); lst != anims.constEnd(); ++lst ) {
        KConfigGroup grp( config.config(), *lst );
        SpriteDef *obj = new SpriteDef( grp );
        mAvailable.append( obj );
    }
}

QRectF SpriteObject::boundingRect() const
{
    return frames.at(currentFrame).boundingRect;
}

void SpriteObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(0, 0, *frames.at(currentFrame).pixmap);
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
