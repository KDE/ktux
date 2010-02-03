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
#include "spritemisc.h"

#include <krandom.h>


QSize SpriteRange::mFieldSize;


SpriteRange::SpriteRange(const QString &str)
{
    set(str);
}


SpriteRange::SpriteRange(int l, int h)
  : mMin(l),
    mMax(h)
{
}


int SpriteRange::random() const
{
    int val = mMin;

    if( mMax != mMin ) {
        val = (KRandom::random()%(mMax-mMin)) + mMin;
    }

    return val;
}


void SpriteRange::set(const QString &str)
{
    int r = str.indexOf("..");

    if( r > 0 ) {
        mMin = parse(str.left(r));
        mMax = parse(str.mid(r+2, 10));
    }
    else {
        mMin = mMax = parse(str);
    }
}


void SpriteRange::setFieldSize(const QSize &size)
{
    mFieldSize = size;
}


int SpriteRange::parse(const QString &str)
{
    int val = 0;

    if( str == "WIDTH" ) {
        val = mFieldSize.width();
    }
    else if( str == "HEIGHT" ) {
        val = mFieldSize.height();
    }
    else {
        val = str.toInt();
    }

    return val;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
