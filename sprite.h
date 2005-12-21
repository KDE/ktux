//-----------------------------------------------------------------------------
//
// ksprite - QCanvas based screensaver
//
// Copyright (c)  Martin R. Jones 1996
//

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <qtimer.h>
#include <qpushbutton.h>
#include <q3ptrlist.h>
#include <q3strlist.h>
#include <q3canvas.h>
//Added by qt3to4:
#include <QTimerEvent>
#include <QVector>
#include <kscreensaver.h>
#include "spriteanim.h"

//-----------------------------------------------------------------------------
class KSpriteSaver : public KScreenSaver
{
	Q_OBJECT
public:
    KSpriteSaver( WId id );
    virtual ~KSpriteSaver();

    void setSpeed(int speed);

private:
    void readSettings();
    void initialise();
    void blank();

protected slots:
    void slotTimeout();

protected:
    virtual void timerEvent(QTimerEvent *);

protected:
    Q3Canvas	*mCanvas;
    Q3CanvasView	*mView;
    QTimer	mTimer;
    int                 mSpeed;
    QList<SpriteGroup*>  mGroups;
    QVector<int>    mTimerIds;
};

class KSpriteSetup : public QDialog
{
    Q_OBJECT
public:
    KSpriteSetup( QWidget *parent = NULL, const char *name = NULL );
    ~KSpriteSetup();

protected:
    void readSettings();

private slots:
    void slotSpeed(int s);
    void slotOkPressed();
    void slotAbout();

private:
    int     speed;
    QWidget *preview;
    KSpriteSaver *saver;
};


#endif

