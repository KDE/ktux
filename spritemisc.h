//-----------------------------------------------------------------------------
//
// spritemisc
//
// Copyright (c)  Martin R. Jones 1999
//

#ifndef __SPRITEMISC_H__
#define __SPRITEMISC_H__

#include <qstring.h>
#include <qsize.h>

class SpriteRange
{
public:
    SpriteRange(const QString &str);
    SpriteRange(int l=0, int h=0) : mMin(l), mMax(h) {}

    int min() const { return mMin; }
    int max() const { return mMax; }

    int random() const;

    void set(int l, int h) { mMin=l; mMax=h; }
    void set(const QString &str);

    static void setFieldSize(const QSize &size);
    static QSize fieldSize() { return mFieldSize; }

protected:
    int parse(const QString &str);

protected:
    int             mMin;
    int             mMax;
    static QSize    mFieldSize;
};

#endif

