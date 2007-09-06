//---------------------------------------------------------------------------
//
// spritepm.cpp
//
// Copyright (c) 1999 Martin R. Jones <mjones@kde.org>
//

#include "spritepm.h"

#include <stdlib.h>
#include <kdebug.h>

#include <QtGui/QPixmap>
#include <Qt3Support/Q3PtrList>
#include <QtCore/QVector>

// static
SpritePixmapManager *SpritePixmapManager::mManager = 0;

//---------------------------------------------------------------------------
//
// Constructor
//
SpritePixmapManager::SpritePixmapManager()
    : mPixmapDir(".")
{
    mPixmaps.setAutoDelete(true);
}

//---------------------------------------------------------------------------
//
// Destructor
//
SpritePixmapManager::~SpritePixmapManager()
{
}

//---------------------------------------------------------------------------
//
// Load an image into the image manager
//
// Returns:
//   pointer to pixmap if loaded successfully, 0 otherwise.
//
const QPixmap *SpritePixmapManager::load(const QString & img)
{
    QPixmap *pixmap = mPixmaps.find(img);

    if (!pixmap)
    {
        // pixmap has not yet been loaded.
        kDebug() << "Reading pixmap: " << img;
        QString path = mPixmapDir + QString("/") + img;
        pixmap = new QPixmap(path);

        if (!pixmap->isNull())
        {
            mPixmaps.insert(img,pixmap);
        }
        else
        {
            delete pixmap;
            pixmap = 0;
            kDebug() << "read failed";;
        }
    }

    return pixmap;
}

//---------------------------------------------------------------------------
//
// returns a pointer to the pixmap manager.
//
SpritePixmapManager *SpritePixmapManager::manager()
{
    if (!mManager)
    {
        mManager = new SpritePixmapManager();
    }

    return mManager;
}

//===========================================================================
//
SpritePixmapSequence::SpritePixmapSequence(Q3PtrList<QPixmap> pm, Q3PtrList<QPoint> hs,
        QVector<int> d)
    : Q3CanvasPixmapArray(pm, hs), mDelays(d)
{
}

// static
SpriteSequenceManager *SpriteSequenceManager::mManager = 0;

//===========================================================================
//
// Constructor
//
SpriteSequenceManager::SpriteSequenceManager()
{
    mSprites.setAutoDelete(true);
}

//---------------------------------------------------------------------------
//
// Destructor
//
SpriteSequenceManager::~SpriteSequenceManager()
{
}

//---------------------------------------------------------------------------
//
// Load an image into the sprite manager
//
// Returns:
//   pointer to sprite if loaded successfully, 0 otherwise.
//
SpritePixmapSequence *SpriteSequenceManager::load(KConfigBase &config,
    const QString & name)
{
    SpritePixmapSequence *sprite = mSprites.find(name);

    if (!sprite)
    {
        kDebug() << "Reading sprite: " << name;
        config.setGroup(name);
        sprite = read(config);
        if (sprite)
        {
            mSprites.insert(name, sprite);
        }
    }

    return sprite;
}

//---------------------------------------------------------------------------
//
SpritePixmapSequence *SpriteSequenceManager::read(KConfigBase &config)
{
    QStringList strImages;
    QStringList strDelays;
    Q3PtrList<QPixmap> pixmaps;
    Q3PtrList<QPoint> hotspots;

    strImages = config.readEntry("Images",QStringList());
    strDelays = config.readEntry("Delays",QStringList());

    QVector<int> delays(strImages.count());

    for (int i = 0; i < strImages.count(); i++)
    {
        const QPixmap *pixmap =
                    SpritePixmapManager::manager()->load(strImages.at(i));
        if (pixmap)
        {
            pixmaps.append(pixmap);
            hotspots.append(new QPoint(0,0));
            delays[i] = strDelays.at(i).toInt();
        }
    }

    return new SpritePixmapSequence(pixmaps, hotspots, delays);
}

//---------------------------------------------------------------------------
//
// returns a pointer to the sprite manager.
//
SpriteSequenceManager *SpriteSequenceManager::manager()
{
    if (!mManager)
    {
        mManager = new SpriteSequenceManager();
    }

    return mManager;
}

