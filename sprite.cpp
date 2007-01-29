//-----------------------------------------------------------------------------
//
// KTux - QCanvas based screensaver
//
// Copyright Martin R. Jones 1999 <mjones@kde.org>
//

#include <stdlib.h>
#include <time.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <qslider.h>
#include <QPixmap>
#include <QTimerEvent>
#include <QVBoxLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>
#include <KStandardGuiItem>
#include <kbuttonbox.h>
#include <kguiitem.h>

#include "spritepm.h"
#include "spritemisc.h"
#include "sprite.h"
#include "sprite.moc"
#include <krandom.h>

// libkscreensaver interface
class KSpriteSaverInterface : public KScreenSaverInterface
{


public:
    virtual KAboutData* aboutData() {
        return new KAboutData( "ktux", I18N_NOOP( "Tux Screen Saver" ), "1.0.0", I18N_NOOP( "Tux Screen Saver" ) );
    }


    virtual KScreenSaver* create( WId id )
    {
        return new KSpriteSaver( id );
    }

    virtual QDialog* setup()
    {
        return new KSpriteSetup();
    }
};

int main( int argc, char *argv[] )
{
    KSpriteSaverInterface kss;
    return kScreenSaverMain( argc, argv, kss );
}

//-----------------------------------------------------------------------------

KSpriteSetup::KSpriteSetup( QWidget *parent, const char *name )
  : QDialog( parent, name, true )
{
    KGlobal::locale()->insertCatalog("ktux");
    saver = 0;

    readSettings();

    setWindowTitle(i18n("Setup KTux") );

    QVBoxLayout *tl = new QVBoxLayout(this);
    tl->setSpacing(10);
    tl->setMargin(10);
    QHBoxLayout *tl1 = new QHBoxLayout;
    tl->addLayout(tl1);
    QVBoxLayout *tl11 = new QVBoxLayout();
    tl11->setSpacing( 5 );
    tl1->addLayout(tl11);

    QLabel *label = new QLabel( i18n("Speed:"), this );
    label->setMinimumSize(label->sizeHint());
    tl11->addStretch(1);
    tl11->addWidget(label);

    QSlider *sb = new QSlider(0, 100, 10, speed, Qt::Horizontal, this );
    tl11->addWidget(sb);
    connect( sb, SIGNAL( valueChanged( int ) ), SLOT( slotSpeed( int ) ) );

    preview = new QWidget( this );
    preview->setFixedSize( 220, 170 );
    preview->setBackgroundColor( Qt::black );
    preview->show();    // otherwise saver does not get correct size
    saver = new KSpriteSaver( preview->winId() );
    tl1->addWidget(preview);

    KButtonBox *bbox = new KButtonBox(this);
    QPushButton *button = bbox->addButton( i18n("About"));
    connect( button, SIGNAL( clicked() ), SLOT(slotAbout() ) );
    bbox->addStretch(1);

    button = bbox->addButton( KStandardGuiItem::ok());
    connect( button, SIGNAL( clicked() ), SLOT( slotOkPressed() ) );

    button = bbox->addButton(KStandardGuiItem::cancel());
    connect( button, SIGNAL( clicked() ), SLOT( reject() ) );
    bbox->layout();
    tl->addWidget(bbox);

    tl->freeze();
}

KSpriteSetup::~KSpriteSetup()
{
    delete saver;
}

// read settings from config file
void KSpriteSetup::readSettings()
{
    KSharedConfig::Ptr config = KGlobal::config();
    config->setGroup( "Settings" );

    speed = config->readEntry( "Speed", 50 );
    if (speed > 100)
	speed = 100;
    else if (speed < 0)
	speed = 0;
}

void KSpriteSetup::slotSpeed(int s)
{
    speed = s;
    if (saver)
	saver->setSpeed(speed);
}

// Ok pressed - save settings and exit
void KSpriteSetup::slotOkPressed()
{
    KSharedConfig::Ptr config = KGlobal::config();
    config->setGroup("Settings");
    config->writeEntry("Speed", speed);
    config->sync();
    accept();
}

void KSpriteSetup::slotAbout()
{
  QMessageBox::message(i18n("About KTux"),
    i18n("KTux Version 1.0\n\nWritten by Martin R. Jones 1999\nmjones@kde.org"),
    i18n("OK"));
}


//-----------------------------------------------------------------------------

KSpriteSaver::KSpriteSaver( WId id ) : KScreenSaver( id )
{
    KGlobal::dirs()->addResourceType("sprite", KStandardDirs::kde_default("data") + "ktux/sprites/");

    initialise();
    readSettings();
    blank();

    connect(&mTimer, SIGNAL(timeout()), SLOT(slotTimeout()));
    mTimer.setSingleShot(true);
    mTimer.start(120-mSpeed);
}

//-----------------------------------------------------------------------------
KSpriteSaver::~KSpriteSaver()
{
    mTimer.stop();
    delete mView;
    delete mCanvas;
}

//-----------------------------------------------------------------------------
//
void KSpriteSaver::setSpeed(int speed)
{
    mSpeed = speed;
    mTimer.start(120-mSpeed);
}

//-----------------------------------------------------------------------------
// read settings from config file
//
void KSpriteSaver::readSettings()
{
    QString str;

    KSharedConfig::Ptr config = KGlobal::config();
    config->setGroup("Settings");

    mSpeed = config->readEntry("Speed", 50);

    QString path = KGlobal::dirs()->findResourceDir( "sprite", "bg.png" );

    SpritePixmapManager::manager()->setPixmapDir(path);

    path += "spriterc";

    KSimpleConfig *mConfig = new KSimpleConfig(path, true);
    mConfig->setGroup("Config");
    QStringList list;
    list = mConfig->readEntry("Groups",list);
    mTimerIds.resize(list.count());
    for (int i = 0; i < list.count(); i++)
    {
	kDebug() << "Group: " << list.at(i) << endl;;
	mConfig->setGroup(list.at(i));
	SpriteGroup *obj = new SpriteGroup(mCanvas, *mConfig);
	mTimerIds[i] = startTimer(obj->refreshTime());
	mGroups.append(obj);
    }
    delete mConfig;
}

//-----------------------------------------------------------------------------
void KSpriteSaver::initialise()
{
    mCanvas = new Q3Canvas();
    QPixmap pm( KStandardDirs::locate("sprite", "bg.png") );
    mCanvas->setBackgroundPixmap( pm );
    mCanvas->resize( width(), height() );
    mView = new Q3CanvasView(mCanvas);
    mView->viewport()->setBackgroundColor( Qt::black );
    mView->resize( size());
    mView->setFrameStyle( QFrame::NoFrame );
    mView->setVScrollBarMode( Q3ScrollView::AlwaysOff );
    mView->setHScrollBarMode( Q3ScrollView::AlwaysOff );
    embed( mView );
    mView->show();
    SpriteRange::setFieldSize(mView->size());
}

//-----------------------------------------------------------------------------
void KSpriteSaver::slotTimeout()
{
    mTimer.setSingleShot(true);
    mTimer.start(120-mSpeed);
    SpriteGroup *grp;

	Q_FOREACH( grp, mGroups )
    {
	grp->next();
    }

    mCanvas->advance();
}

//-----------------------------------------------------------------------------
void KSpriteSaver::timerEvent(QTimerEvent *ev)
{
    for (int i = 0; i < mTimerIds.size(); i++)
    {
	if (mTimerIds[i] == ev->timerId())
	{
	    mGroups.at(i)->refresh();
	    killTimer(ev->timerId());
	    mTimerIds[i] = startTimer(mGroups.at(i)->refreshTime());
	    break;
	}
    }
}

//-----------------------------------------------------------------------------
void KSpriteSaver::blank()
{
    setBackgroundColor( Qt::black );
    update();
}

