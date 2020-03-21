#include "ktelephone.h"
#include "ktelephoneguide.h"
#include "ktelephonemanager.h"

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

#include <QDebug>

KTelephoneManager::KTelephoneManager(QWidget *parent) :
    QWidget(parent)
{
  this->trayIcon = new QSystemTrayIcon(this);
  this->trayIcon->setToolTip("Telephone");

  this->trayIconMenu = new QMenu(this);
  const auto openAction = this->trayIconMenu->addAction(tr("Open"));
  const auto exitAction = this->trayIconMenu->addAction(tr("Exit"));
  this->trayIcon->setContextMenu(this->trayIconMenu);
  this->trayIconMenu->show();

  const auto appIcon = QIcon("data/ktelephone.png");
  this->trayIcon->setIcon(appIcon);
  this->setWindowIcon(appIcon);

  connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
  connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  this->trayIcon->show();

  this->connectDatabase();
  this->bootstrapDatabase();
  mUAManager = new UserAgentManager(this);
  this->bootstrap();
}

KTelephoneManager::~KTelephoneManager()
{
  this->unloadKTelephones();

  if (mTelephone) {
    delete mTelephone;
  }
  if (mGuide) {
    delete mGuide;
  }
  if (mUAManager) {
    delete mUAManager;
  }
}

void KTelephoneManager::bootstrap()
{
  this->loadFromDatabase();
  if (!telephones.keys().length()) {
    this->startGuide();
    return;
  }
}

void KTelephoneManager::open()
{
  this->bootstrap();
}

UserAgentManager* KTelephoneManager::getUserAgentManager()
{
  return mUAManager;
}

void KTelephoneManager::newKTelephone(Telephone_t telephone)
{
  mTelephone = new KTelephone();
  mTelephone->setManager(this);
  mTelephone->setTelephone(telephone);
  telephones[telephone.username] = mTelephone;
  if (telephone.active == 1) {
    mTelephone->show();
    AccountConfig acfg = mUAManager->getAccountConfig(telephone);
    mUAManager->newUserAgent(mTelephone,
                             telephone.username,
                             acfg);
    mTelephone->statusMessage("Registering...");
  }
  mTelephone = NULL;
  qDebug() << telephones;
}

void KTelephoneManager::updateKTelephone(QString oldUsername, Telephone_t telephone)
{
  mTelephone = telephones[oldUsername];
  mTelephone->setTelephone(telephone);
  if (QString::compare(telephone.username, oldUsername)) {
    telephones.remove(oldUsername);
  }
  telephones[telephone.username] = mTelephone;
  this->updateTelephone(telephone);
  qDebug() << telephones;

  mUAManager->removeUserAgent(oldUsername);
  if (telephone.active == 1) {
    mUAManager->newUserAgent(mTelephone,
                             telephone.username,
                             mUAManager->getAccountConfig(telephone));
    mTelephone->statusMessage("Registering...");
  }
  mTelephone = NULL;
}

void KTelephoneManager::removeKTelephone(Telephone_t telephone)
{
  telephones.remove(telephone.username);
  mUAManager->removeUserAgent(telephone.username);
  qDebug() << telephones;
  mTelephone = NULL;
}

QHash<QString, KTelephone*> KTelephoneManager::getTelephones()
{
  return telephones;
}

void KTelephoneManager::connectDatabase()
{
  const QString DRIVER("QSQLITE");
  if(QSqlDatabase::isDriverAvailable(DRIVER)) {
    QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
    db.setDatabaseName("ktelephone.db");
    if(!db.open())
      qWarning() << "ERROR: " << db.lastError();
  }
}

void KTelephoneManager::bootstrapDatabase()
{
  QSqlQuery query("CREATE TABLE IF NOT EXISTS telephones (id INTEGER PRIMARY KEY AUTOINCREMENT, description TEXT, name TEXT, domain TEXT, username TEXT, password TEXT, active integer)");
  if(!query.isActive())
    qWarning() << "KTelephoneManager::bootstrapDatabase - ERROR: " << query.lastError().text();
}

void KTelephoneManager::loadFromDatabase()
{
  QSqlQuery query;
  query.prepare("SELECT id, description, name, domain, username, password, active FROM telephones;");

  if(!query.exec()) {
    qWarning() << "KTelephoneManager::loadFromDatabase - ERROR: " << query.lastError().text();
    return;
  }

  while(query.next()) {
    Telephone_t telephone {
      query.value(0).toString(),
      query.value(1).toString(),
      query.value(2).toString(),
      query.value(3).toString(),
      query.value(4).toString(),
      query.value(5).toString(),
      query.value(6).toInt(),
    };

    this->newKTelephone(telephone);
  }
}

void KTelephoneManager::unloadKTelephones()
{
  const QHash<QString, KTelephone*> telephones = this->getTelephones();
  foreach( QString item, telephones.keys() ) {
    mUAManager->removeUserAgent(item);
  }
}

void KTelephoneManager::saveTelephone(Telephone_t* telephone)
{
  QSqlQuery query;
  query.prepare("INSERT INTO telephones (description, name, domain, username, password, active) VALUES (:description, :name, :domain, :username, :password, :active)");
  query.bindValue(":description", telephone->description);
  query.bindValue(":name", telephone->name);
  query.bindValue(":domain", telephone->domain);
  query.bindValue(":username", telephone->username);
  query.bindValue(":password", telephone->password);
  query.bindValue(":active", telephone->active);

  if(!query.exec()) {
    qWarning() << "KTelephoneManager::saveTelephone - ERROR: " << query.lastError().text();
    return;
  }

  if (query.lastInsertId().canConvert(QMetaType::QString)) {
    telephone->id = query.lastInsertId().toString();
  }
}

void KTelephoneManager::updateTelephone(Telephone_t telephone)
{
  if (telephone.id.isEmpty() || telephone.id.isNull()) {
    qWarning() << "No id provided on update.";
    return;
  }

  QSqlQuery query;
  query.prepare("UPDATE telephones SET description=:description, name=:name, domain=:domain, username=:username, password=:password, active=:active WHERE id = :id");
  query.bindValue(":description", telephone.description);
  query.bindValue(":name", telephone.name);
  query.bindValue(":domain", telephone.domain);
  query.bindValue(":username", telephone.username);
  query.bindValue(":password", telephone.password);
  query.bindValue(":active", telephone.active);
  query.bindValue(":id", telephone.id);

  qDebug() << telephone.id;

  if(!query.exec()) {
    qWarning() << "KTelephoneManager::updateTelephone - ERROR: " << query.lastError().text();
    return;
  }
}

void KTelephoneManager::deleteTelephone(Telephone_t telephone)
{
  if (telephone.id.isEmpty() || telephone.id.isNull()) {
    qWarning() << "No id provided on delete.";
    return;
  }

  QSqlQuery query;
  query.prepare("DELETE FROM telephones WHERE id = :id");
  query.bindValue(":id", telephone.id);

  if(!query.exec()) {
    qWarning() << "KTelephoneManager::deleteTelephone - ERROR: " << query.lastError().text();
    return;
  }
}

void KTelephoneManager::openPreferences(void) {
  mPreferences = new KTelephonePreferences(this);
  mPreferences->setManager(this);
  mPreferences->show();
}

void KTelephoneManager::closePreferences() {
  delete mPreferences;
  mPreferences = NULL;
}

void KTelephoneManager::startGuide()
{
  mGuide = new KTelephoneGuide();
  mGuide->setManager(this);
  mGuide->show();
}
