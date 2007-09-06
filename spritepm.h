//---------------------------------------------------------------------------
//
// spritepm.h
//
// Copyright (c) 1999 Martin R. Jones <mjones@kde.org>
//

#ifndef SPRITEPM_H
#define SPRITEPM_H

#include <Qt3Support/Q3Dict>
#include <QtGui/QPixmap>
#include <Qt3Support/Q3Canvas>
#include <Qt3Support/Q3PtrList>
#include <QtCore/QVector>

#include <kconfigbase.h>
#include <kconfig.h>

//---------------------------------------------------------------------------
//
// SpritePixmapManager stores the frames used in animations.
//
class SpritePixmapManager
{
public:
    SpritePixmapManager();
    virtual ~SpritePixmapManager();

    void setPixmapDir(const QString &dir)
        { mPixmapDir = dir; }
    void reset()
        { mPixmapDir = "."; mPixmaps.clear(); }
    const QPixmap *load(const QString & img);
    const QPixmap *pixmap(const char *img) const
        { return mPixmaps.find(img); }

    static SpritePixmapManager *manager();

public:
    QString        mPixmapDir;           // get pixmaps from here
    Q3Dict<QPixmap> mPixmaps;             // list of pixmaps
    static SpritePixmapManager *mManager; // static pointer to instance
};

//---------------------------------------------------------------------------
//
class SpritePixmapSequence : public Q3CanvasPixmapArray
{
public:
    SpritePixmapSequence(Q3PtrList<QPixmap> pm, Q3PtrList<QPoint> hs, QVector<int> d);

    int delay(int i) const { return mDelays[i]; }

protected:
    QVector<int> mDelays;
};

//---------------------------------------------------------------------------
//
// SpriteManager stores the animation sequences
//
class SpriteSequenceManager
{
public:
    SpriteSequenceManager();
    ~SpriteSequenceManager();
 
    SpritePixmapSequence *load(KConfigBase &config, const QString & name);
    SpritePixmapSequence *sprite(const char *name)
        { return mSprites.find(name); }

    static SpriteSequenceManager *manager();
     
protected:
    SpritePixmapSequence *read(KConfigBase &config);

protected:
    Q3Dict<SpritePixmapSequence> mSprites;
    static SpriteSequenceManager *mManager;
};

#endif // SPRITEPM_H

