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

#include <stdlib.h>
#include <kdebug.h>
#include <kconfiggroup.h>

#include <QPixmap>
#include <QVector>
#include <Qt3Support/Q3PtrList>


// static
SpritePixmapManager *SpritePixmapManager::mManager = 0;


SpritePixmapManager::SpritePixmapManager()
  : mPixmapDir(".")
{
    mPixmaps.setAutoDelete(true);
}


SpritePixmapManager::~SpritePixmapManager()
{
}


const QPixmap *SpritePixmapManager::load(const QString & img)
{
    QPixmap *pixmap = mPixmaps.find(img);

    if( !pixmap ) {
        // pixmap has not yet been loaded.
        kDebug() << "Reading pixmap: " << img;
        QString path = mPixmapDir + QString("/") + img;
        pixmap = new QPixmap(path);

        if( !pixmap->isNull() ) {
            mPixmaps.insert(img,pixmap);
        }
        else {
            delete pixmap;
            pixmap = 0;
            kDebug() << "read failed";;
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


SpritePixmapSequence::SpritePixmapSequence(Q3PtrList<QPixmap> pm, Q3PtrList<QPoint> hs, QVector<int> d)
  : Q3CanvasPixmapArray( pm, hs ),
    mDelays( d )
{
}

SpriteSequenceManager *SpriteSequenceManager::mManager = 0;



SpriteSequenceManager::SpriteSequenceManager()
{
    mSprites.setAutoDelete( true );
}


SpriteSequenceManager::~SpriteSequenceManager()
{
}


SpritePixmapSequence* SpriteSequenceManager::load(KConfigBase &config, const QString &name)
{
    SpritePixmapSequence *sprite = mSprites.find( name );

    if( !sprite ) {
        kDebug() << "Reading sprite: " << name;
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
    Q3PtrList<QPixmap> pixmaps;
    Q3PtrList<QPoint> hotspots;

    strImages = config.readEntry( "Images",QStringList() );
    strDelays = config.readEntry( "Delays",QStringList() );

    QVector<int> delays( strImages.count() );

    for( int i = 0; i < strImages.count(); i++ ) {
        const QPixmap *pixmap =
                    SpritePixmapManager::manager()->load( strImages.at( i ) );
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
