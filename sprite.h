//-----------------------------------------------------------------------------
//
// ksprite - QCanvas based screensaver
//
// Copyright Martin R. Jones 1996 <mjones@kde.org>
//

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <QtCore/QTimer>
#include <QtGui/QPushButton>
#include <Qt3Support/Q3Canvas>
#include <QtCore/QTimerEvent>
#include <QtCore/QVector>
#include <KDialog>
#include <kscreensaver.h>

#include "spriteanim.h"

//-----------------------------------------------------------------------------
class KSpriteSaver : public KScreenSaver
{
	Q_OBJECT
public:
    explicit KSpriteSaver( WId id );
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

class KSpriteSetup : public KDialog
{
    Q_OBJECT
public:
    explicit KSpriteSetup( QWidget *parent = NULL, const char *name = NULL );
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

