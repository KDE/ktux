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
#include <qlist.h>
#include <qstrlist.h>
#include <qcanvas.h>
#include "saver.h"
#include "spriteanim.h"

//-----------------------------------------------------------------------------
class KSpriteSaver : public kScreenSaver
{
	Q_OBJECT
public:
    KSpriteSaver( Drawable drawable );
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
    QCanvas	*mSpriteField;
    QCanvasView	*mView;
    QTimer	mTimer;
    int                 mSpeed;
    QList<SpriteGroup>  mGroups;
    QArray<int>    mTimerIds;
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

