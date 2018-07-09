#include "ktelephonemanager.h"
#include "ktelephone.h"
#include "ui_ktelephone.h"

#include <QDebug>

KTelephone::KTelephone(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ktelephone)
{
  ui->setupUi(this);
    
  connect(ui->actionPreferences,
          SIGNAL(triggered()), this,
          SLOT(actionPreferences()));
  connect(ui->actionAbout,
          SIGNAL(triggered()), this,
          SLOT(actionAbout()));
}

KTelephone::~KTelephone()
{
  delete ui;
}

void KTelephone::setManager(KTelephoneManager *manager)
{
  mManager = manager;
}

KTelephoneManager* KTelephone::getManager()
{
  return mManager;
}

void KTelephone::setTelephone(Telephone_t telephone)
{
  mTelephone = telephone;
}

void KTelephone::actionPreferences()
{
  preferences = new KTelephonePreferences(this);
  preferences->show();
}

void KTelephone::actionAbout()
{
  qDebug() << "about";
}
