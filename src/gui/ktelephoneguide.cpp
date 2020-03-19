#include "ktelephonemanager.h"

#include "ktelephoneguide.h"
#include "ui_startguide.h"
#include <QDebug>

KTelephoneGuide::KTelephoneGuide(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::guide)
{
  ui->setupUi(this);

  connect(ui->buttonBox,
          SIGNAL(accepted()), this,
          SLOT(accept()), Qt::UniqueConnection);
}

KTelephoneGuide::~KTelephoneGuide()
{
  delete ui;
}

void KTelephoneGuide::setManager(KTelephoneManager *manager)
{
  mManager = manager;
}

void KTelephoneGuide::accept()
{
  if (ui->descriptionEdit->text() == '\0' ||
      ui->nameEdit->text() == '\0' ||
      ui->domainEdit->text() == '\0' ||
      ui->usernameEdit->text() == '\0' ||
      ui->passwordEdit->text() == '\0') {
    return;
  }

  Telephone_t telephone {
    QString("0"),
    ui->descriptionEdit->text(),
    ui->nameEdit->text(),
    ui->domainEdit->text(),
    ui->usernameEdit->text(),
    ui->passwordEdit->text(),
    1
  };

  mManager->saveTelephone(&telephone);
  mManager->newKTelephone(telephone);
  this->close();
}
