//-----------------------------------------------------------------------------
//
// spritemisc
//
// Copyright (c)  Martin R. Jones 1999
//

#include <kapplication.h>
#include "spritemisc.h"

QSize SpriteRange::mFieldSize;

SpriteRange::SpriteRange(const QString &str)
{
    set(str);
}

int SpriteRange::random() const
{
    int val = mMin;

    if (mMax != mMin)
    {
        val = (KApplication::random()%(mMax-mMin)) + mMin;
    }

    return val;
}

void SpriteRange::set(const QString &str)
{
    int r = str.find("..");

    if (r > 0)
    {
        mMin = parse(str.left(r));
        mMax = parse(str.mid(r+2, 10));
    }
    else
    {
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

    if (str == "WIDTH")
    {
        val = mFieldSize.width();
    }
    else if (str == "HEIGHT")
    {
        val = mFieldSize.height();
    }
    else
    {
        val = str.toInt();
    }

    return val;
}

