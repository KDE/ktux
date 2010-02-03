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
#ifndef SPRITEPM_H
#define SPRITEPM_H

#include <Qt3Support/Q3Dict>
#include <QtGui/QPixmap>
#include <Qt3Support/Q3Canvas>
#include <Qt3Support/Q3PtrList>
#include <QtCore/QVector>

#include <kconfigbase.h>
#include <kconfig.h>


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


class SpritePixmapSequence : public Q3CanvasPixmapArray
{
    public:
        SpritePixmapSequence(Q3PtrList<QPixmap> pm, Q3PtrList<QPoint> hs, QVector<int> d);

        int delay(int i) const { return mDelays[i]; }

    protected:
        QVector<int> mDelays;
};

class KConfigGroup;


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
        SpritePixmapSequence *read(const KConfigGroup &config);

    protected:
        Q3Dict<SpritePixmapSequence> mSprites;
        static SpriteSequenceManager *mManager;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
