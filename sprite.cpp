//-----------------------------------------------------------------------------
//
// ksprite - QCanvas based screensaver
//
// Copyright (c)  Martin R. Jones 1999
//

#include <stdlib.h>
#include <time.h>
#include <qcolor.h>
#include <qlabel.h>
#include <qgrpbox.h>
#include <qmsgbox.h>
#include <qslider.h>
#include <qlayout.h>
#include <kapp.h>
#include <kglobal.h>
#include <kstddirs.h>
#include <klocale.h>
#include <qxembed.h>

#include <kbuttonbox.h>

#include "kcolordlg.h"
#include "spritepm.h"
#include "spritemisc.h"
#include "sprite.h"

#include "sprite.moc"

#include <X11/Xlib.h>

static KSpriteSaver *saver = NULL;

//-----------------------------------------------------------------------------
// standard screen saver interface functions
//
void startScreenSaver(Drawable d)
{
  srandom(time(0));
  if (saver)
    return;
  saver = new KSpriteSaver(d);
}

void stopScreenSaver()
{
  if (saver)
    delete saver;
  saver = 0;
}

int setupScreenSaver()
{
  KSpriteSetup dlg;

  return dlg.exec();
}

//-----------------------------------------------------------------------------

KSpriteSetup::KSpriteSetup( QWidget *parent, const char *name )
  : QDialog( parent, name, TRUE )
{
  saver = 0;

  readSettings();

  setCaption(i18n("Setup ksprite") );

  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  QHBoxLayout *tl1 = new QHBoxLayout;
  tl->addLayout(tl1);
  QVBoxLayout *tl11 = new QVBoxLayout(5);
  tl1->addLayout(tl11);	

  QLabel *label = new QLabel( i18n("Speed:"), this );
  label->setMinimumSize(label->sizeHint());
  tl11->addStretch(1);
  tl11->addWidget(label);

  QSlider *sb = new QSlider(20, 100, 10, speed, QSlider::Horizontal, this );
  sb->setMinimumWidth( 180);
  sb->setFixedHeight(20);
  tl11->addWidget(sb);
  connect( sb, SIGNAL( valueChanged( int ) ), SLOT( slotSpeed( int ) ) );

  preview = new QWidget( this );
  preview->setFixedSize( 220, 170 );
  preview->setBackgroundColor( black );
  preview->show();    // otherwise saver does not get correct size
  saver = new KSpriteSaver( preview->winId() );
  tl1->addWidget(preview);

  KButtonBox *bbox = new KButtonBox(this);	
  QButton *button = bbox->addButton( i18n("About"));
  connect( button, SIGNAL( clicked() ), SLOT(slotAbout() ) );
  bbox->addStretch(1);

  button = bbox->addButton( i18n("OK"));	
  connect( button, SIGNAL( clicked() ), SLOT( slotOkPressed() ) );

  button = bbox->addButton(i18n("Cancel"));
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
  KConfig *config = KApplication::kApplication()->config();
  config->setGroup( "Settings" );

  speed = config->readNumEntry( "Speed", 50 );
  if (speed > 100)
    speed = 100;
  else if (speed < 20)
    speed = 20;
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
  KConfig *config = KApplication::kApplication()->config();
  config->setGroup("Settings");
  config->writeEntry("Speed", speed);
  config->sync();
  accept();
}

void KSpriteSetup::slotAbout()
{
  QMessageBox::message(i18n("About Sprite"), 
    i18n("Sprite Version 0.1\n\nwritten by Martin R. Jones 1999\nmjones@kde.org"), 
    i18n("OK"));
}


//-----------------------------------------------------------------------------

KSpriteSaver::KSpriteSaver(Drawable drawable) : kScreenSaver(drawable)
{
  initialise();
  readSettings();
  blank();

  connect(&mTimer, SIGNAL(timeout()), SLOT(slotTimeout()));
  mTimer.start(mSpeed, true);
}

//-----------------------------------------------------------------------------
KSpriteSaver::~KSpriteSaver()
{
  mTimer.stop();
  delete mView;
  delete mSpriteField;
}

//-----------------------------------------------------------------------------
//
void KSpriteSaver::setSpeed(int speed)
{
  mSpeed = speed;
  mTimer.changeInterval(mSpeed);
}

//-----------------------------------------------------------------------------
// read settings from config file
//
void KSpriteSaver::readSettings()
{
  QString str;

  KConfig *config = KApplication::kApplication()->config();
  config->setGroup("Settings");

  mSpeed = config->readNumEntry("Speed", 50);

  QString path = KGlobal::dirs()->findResourceDir( "sprite", "bg.png" );

  SpritePixmapManager::manager()->setPixmapDir(path);

  path += "spriterc";

  debug("path = " + path);

  KSimpleConfig *mConfig = new KSimpleConfig(path, true);
  mConfig->setGroup("Config");
  QStrList list;
  int groups = mConfig->readListEntry("Groups", list);
  mTimerIds.resize(groups);
  for (int i = 0; i < groups; i++)
  {
    debug("Group: %s", list.at(i));
    mConfig->setGroup(list.at(i));
    SpriteGroup *obj = new SpriteGroup(mSpriteField, *mConfig);
    mTimerIds[i] = startTimer(obj->refreshTime());
    mGroups.append(obj);
  }
  delete mConfig;
}

//-----------------------------------------------------------------------------
void KSpriteSaver::initialise()
{
  mSpriteField = new QCanvas();
  QPixmap pm( locate("sprite", "bg.png") );
  mSpriteField->setBackgroundPixmap( pm );
  mSpriteField->resize( mWidth, mHeight );
  mView = new QCanvasView(mSpriteField);
  mView->viewport()->setBackgroundColor( black );
  mView->resize( mWidth, mHeight );
  mView->setFrameStyle( QFrame::NoFrame );
  mView->setVScrollBarMode( QScrollView::AlwaysOff );
  mView->setHScrollBarMode( QScrollView::AlwaysOff );
  QXEmbed::embedClientIntoWindow( mView, mDrawable );
  mView->show();
  SpriteRange::setFieldSize(mView->size());
}

//-----------------------------------------------------------------------------
void KSpriteSaver::slotTimeout()
{
  mTimer.start(mSpeed, true);
  SpriteGroup *grp;

  for (grp = mGroups.first(); grp; grp = mGroups.next())
  {
    grp->next();
  }

  mSpriteField->advance();
}

//-----------------------------------------------------------------------------
void KSpriteSaver::timerEvent(QTimerEvent *ev)
{
  for (unsigned i = 0; i < mTimerIds.size(); i++)
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
  XSetWindowBackground(qt_xdisplay(), mDrawable, black.pixel());
  XClearWindow(qt_xdisplay(), mDrawable);
}

