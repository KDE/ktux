//---------------------------------------------------------------------------
//
// spritepm.cpp
//
// Copyright (c) 1999 Martin R. Jones <mjones@kde.org>
//

#include <stdlib.h>
#include "spritepm.h"

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
const QPixmap *SpritePixmapManager::load(const char *img)
{
    QPixmap *pixmap = mPixmaps.find(img);

    if (!pixmap)
    {
        // pixmap has not yet been loaded.
        debug("Reading pixmap: %s", img);
        QString path = mPixmapDir + QString("/") + img;
        pixmap = new QPixmap(path);

        if (!pixmap->isNull())
        {
            mPixmaps.insert(img,pixmap);
            debug("read successfully");
        }
        else
        {
            delete pixmap;
            pixmap = 0;
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
SpritePixmapSequence::SpritePixmapSequence(QList<QPixmap> pm, QList<QPoint> hs,
        QArray<int> d)
    : QCanvasPixmapArray(pm, hs), mDelays(d)
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
    const char *name)
{
    SpritePixmapSequence *sprite = mSprites.find(name);

    if (!sprite)
    {
        debug("Reading sprite: %s", name);
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
    QStrList strImages;
    QStrList strDelays;
    QList<QPixmap> pixmaps;
    QList<QPoint> hotspots;

    int frames = config.readListEntry("Images", strImages);
    config.readListEntry("Delays", strDelays);

    QArray<int> delays(frames);

    for (int i = 0; i < frames; i++)
    {
        const QPixmap *pixmap =
                    SpritePixmapManager::manager()->load(strImages.at(i));
        if (pixmap)
        {
            pixmaps.append(pixmap);
            hotspots.append(new QPoint(0,0));
            delays[i] = atoi(strDelays.at(i));
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

