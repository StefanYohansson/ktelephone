#include "ktelephone.h"
#include "ktelephonepreferences.h"
#include "ui_settings.h"
#include <QDebug>

KTelephonePreferences::KTelephonePreferences(KTelephone *parent) :
    QDialog(parent),
    ui(new Ui::settings)
{
  ui->setupUi(this);

  if (!parent->getManager()) {
    return;
  }

  mTelephones = parent->getManager()->getTelephones();

  qDebug() << mTelephones.keys();

  foreach( QString item, mTelephones.keys() ) {
    ui->telephonesList->addItem(mTelephones[item]->mTelephone.domain);
  }

}

KTelephonePreferences::~KTelephonePreferences()
{
  delete ui;
}

