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

  connect(ui->actionPreferences,
          SIGNAL(triggered()), this,
          SLOT(actionPreferences()));
  connect(ui->actionAbout,
          SIGNAL(triggered()), this,
          SLOT(actionAbout()));
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
