//---------------------------------------------------------------------------
//
// spritepm.h
//
// Copyright (c) 1999 Martin R. Jones <mjones@kde.org>
//

#ifndef SPRITEPM_H
#define SPRITEPM_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <qdict.h>
#include <qpixmap.h>
#include <qcanvas.h>
#include <kconfigbase.h>
#include <ksimpleconfig.h>                                                      

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
    QDict<QPixmap> mPixmaps;             // list of pixmaps
    static SpritePixmapManager *mManager; // static pointer to instance
};

//---------------------------------------------------------------------------
//
class SpritePixmapSequence : public QCanvasPixmapArray
{
public:
    SpritePixmapSequence(QPtrList<QPixmap> pm, QPtrList<QPoint> hs, QMemArray<int> d);

    int delay(int i) const { return mDelays[i]; }

protected:
    QMemArray<int> mDelays;
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
    QDict<SpritePixmapSequence> mSprites;
    static SpriteSequenceManager *mManager;
};

#endif // SPRITEPM_H

