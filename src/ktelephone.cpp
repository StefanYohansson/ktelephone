#include "ktelephonemanager.h"
#include "ktelephone.h"
#include "ui_ktelephone.h"

#include <QDebug>

KTelephone::KTelephone(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ktelephone)
{
  ui->setupUi(this);

  this->setAttribute(Qt::WA_DeleteOnClose);

  statusLabel = new QLabel(this);
  ui->statusBar->addPermanentWidget(statusLabel);

  connect(ui->actionPreferences,
          SIGNAL(triggered()), this,
          SLOT(actionPreferences()));
  connect(ui->actionAbout,
          SIGNAL(triggered()), this,
          SLOT(actionAbout()));
  connect(ui->statusComboBox,
          SIGNAL(currentIndexChanged(int)), this,
          SLOT(changeStatus(int)));
}

KTelephone::~KTelephone()
{
  mManager->getUserAgentManager()->removeUserAgent(mTelephone.domain);
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

void KTelephone::changeRegistrationStatus(bool status)
{
  disconnect(ui->statusComboBox,
             SIGNAL(currentIndexChanged(int)), 0, 0);
  if (status) {
    statusLabel->setText("Registered");
    ui->statusComboBox->setCurrentIndex(0);
  } else {
    statusLabel->setText("Unregistered");
    ui->statusComboBox->setCurrentIndex(1);
  }
  connect(ui->statusComboBox,
          SIGNAL(currentIndexChanged(int)), this,
          SLOT(changeStatus(int)));
}

void KTelephone::statusMessage(QString message)
{
  statusLabel->setText(message);
}

void KTelephone::setTelephone(Telephone_t telephone)
{
  mTelephone = telephone;
  QString title = QString("");
  title.append(mTelephone.username);
  if (!mTelephone.description.isEmpty()) {
    title.append(QString(" - "));
    title.append(mTelephone.description);
  } else {
    title.append(QString(" - "));
    title.append(mTelephone.domain);
  }
  this->setWindowTitle(title);
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

void KTelephone::changeStatus(int index)
{
  switch (index) {
    case 0:
      mManager->getUserAgentManager()->setRegister(mTelephone.domain, true);
      break;
    case 1:
      mManager->getUserAgentManager()->setRegister(mTelephone.domain, false);
      break;
  }
}
