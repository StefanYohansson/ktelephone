#include "ktelephone.h"
#include "ktelephoneguide.h"
#include "ktelephonemanager.h"

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

#include <QDebug>

KTelephoneManager::KTelephoneManager()
{
  this->connectDatabase();
  this->bootstrapDatabase();
  this->loadFromDatabase();
  if (!telephones.keys().length()) {
    this->startGuide();
    return;
  }
}

KTelephoneManager::~KTelephoneManager()
{
  if (mTelephone) {
    delete mTelephone;
  }
  if (mGuide) {
    delete mGuide;
  }
}

void KTelephoneManager::newKTelephone(Telephone_t telephone)
{
  mTelephone = new KTelephone();
  mTelephone->setManager(this);
  mTelephone->setTelephone(telephone);
  mTelephone->show();
  telephones[telephone.domain] = mTelephone;
  mTelephone = NULL;
  qDebug() << telephones;
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

void KTelephoneManager::saveTelephone(Telephone_t telephone)
{
  QSqlQuery query;
  query.prepare("INSERT INTO telephones (description, name, domain, username, password, active) VALUES (:description, :name, :domain, :username, :password, :active)");
  query.bindValue(":description", telephone.description);
  query.bindValue(":name", telephone.name);
  query.bindValue(":domain", telephone.domain);
  query.bindValue(":username", telephone.username);
  query.bindValue(":password", telephone.password);
  query.bindValue(":active", telephone.active);

  if(!query.exec()) {
    qWarning() << "KTelephoneManager::saveTelephone - ERROR: " << query.lastError().text();
    return;
  }

  if (query.lastInsertId().canConvert(QMetaType::QString)) {
    telephone.id = query.lastInsertId().toString();
  }
}

void KTelephoneManager::startGuide()
{
  mGuide = new KTelephoneGuide();
  mGuide->setManager(this);
  mGuide->show();
}
