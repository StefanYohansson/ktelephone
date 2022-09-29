#include "ktelephone.hpp"
#include "ktelephoneguide.hpp"
#include "ktelephonemanager.hpp"
#include "ktelephoneabout.hpp"

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>

#include <QDebug>

const char *KTELEPHONE_CONF_DIR = std::getenv("KTELEPHONE_CONF_DIR");

KTelephoneManager::KTelephoneManager(QWidget *parent) :
		QMainWindow(parent) {
	this->trayIcon = new QSystemTrayIcon(this);
	this->trayIcon->setToolTip("KTelephone");

	this->trayIconMenu = new QMenu(this);
	const auto openAction = this->trayIconMenu->addAction(tr("Open"));
	const auto settingsAction = this->trayIconMenu->addAction(tr("Settings"));
	const auto exitAction = this->trayIconMenu->addAction(tr("Exit"));
	this->trayIcon->setContextMenu(this->trayIconMenu);
	this->trayIconMenu->show();

	const auto appIcon = QIcon("data/ktelephone.png");
	this->trayIcon->setIcon(appIcon);
	this->setWindowIcon(appIcon);

	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
	connect(settingsAction, SIGNAL(triggered()), this, SLOT(openPreferencesSlot()));
	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	this->trayIcon->show();

	this->connectDatabase();
	this->bootstrapDatabase();
	this->bootstrapGlobal();
	mUAManager = new UserAgentManager(this);
	this->bootstrap();
}

KTelephoneManager::~KTelephoneManager() {
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

void KTelephoneManager::bootstrapGlobal() {
	QSqlQuery global_query;
	global_query.prepare("SELECT active_codecs FROM config;");

	if (!global_query.exec()) {
		qWarning() << "KTelephoneManager::loadFromDatabase - ERROR: " << global_query.lastError().text();
		return;
	}

	while (global_query.next()) {
		global.active_codecs = global_query.value(0).toString();
	}
}

void KTelephoneManager::bootstrap() {
	this->loadFromDatabase();
	if (telephones.isEmpty()) {
		this->startGuide();
		return;
	}

	if (!this->hasActiveAccounts()) {
		this->openPreferences();
		return;
	}
}

void KTelephoneManager::openPreferencesSlot() {
	this->openPreferences();
}

UserAgentManager *KTelephoneManager::getUserAgentManager() {
	return mUAManager;
}

void KTelephoneManager::newKTelephone(Telephone_t telephone) {
	if (telephones.contains(telephone.username)) {
		mTelephone = telephones[telephone.username];
	} else {
		mTelephone = new KTelephone();
	}
	mTelephone->setManager(this);
	mTelephone->setTelephone(telephone);
	telephones[telephone.username] = mTelephone;
	if (telephone.active == 1) {
		mTelephone->show();
		AccountConfig acfg = mUAManager->getAccountConfig(&telephone);
		mUAManager->newUserAgent(mTelephone,
		                         telephone.username,
		                         acfg);
		mTelephone->statusMessage("Registering...");
	}
	mTelephone = NULL;
	qDebug() << telephones;
}

void KTelephoneManager::updateKTelephone(QString oldUsername, Telephone_t *telephone) {
	mTelephone = telephones[oldUsername];
	mTelephone->setTelephone(*telephone);
	if (QString::compare(telephone->username, oldUsername)) {
		telephones.remove(oldUsername);
	}
	telephones[telephone->username] = mTelephone;
	this->updateTelephone(telephone);

	mUAManager->removeUserAgent(oldUsername);
	if (mPreferences) {
		mPreferences->reload();
	}
	if (telephone->active == 1) {
		mUAManager->newUserAgent(mTelephone,
		                         telephone->username,
		                         mUAManager->getAccountConfig(telephone));
		mTelephone->statusMessage("Registering...");
	}
	mTelephone = NULL;
}

void KTelephoneManager::removeKTelephone(Telephone_t *telephone) {
	KTelephone *removedTelephone = telephones.value(telephone->username);
	telephones.remove(telephone->username);
	if (telephone->active) {
		removedTelephone->close();
	}
	mUAManager->removeUserAgent(telephone->username);
	if (mPreferences) {
		mPreferences->reload();
	}
	mTelephone = NULL;
}

QHash<QString, KTelephone *> KTelephoneManager::getTelephones() {
	return telephones;
}

void KTelephoneManager::connectDatabase() {
	const QString DRIVER("QSQLITE");
	if (QSqlDatabase::isDriverAvailable(DRIVER)) {
		QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
		QDir confDir = QDir(KTELEPHONE_CONF_DIR);

		if (!confDir.exists()) {
			confDir.mkdir(confDir.path());
		}

		db.setDatabaseName(confDir.filePath("ktelephone.db"));
		if (!db.open()) {
			qWarning() << "ERROR: " << db.lastError();
		}
	}
}

void KTelephoneManager::bootstrapDatabase() {
	QSqlQuery query(
			"CREATE TABLE IF NOT EXISTS telephones (id INTEGER PRIMARY KEY AUTOINCREMENT, description TEXT, name TEXT, domain TEXT, username TEXT, password TEXT, active integer, should_register_startup integer, should_subscribe_presence integer, should_publish_presence integer, should_use_blf integer, should_disable_ringback_tone integer, custom_ringtone TEXT, transport integer, subscription_expiry_delay integer, keep_alive_expiry_delay integer, registration_expiry_delay integer, use_stun integer, stun_server TEXT)");
	if (!query.isActive())
		qWarning() << "KTelephoneManager::bootstrapDatabase - ERROR: " << query.lastError().text();

	QSqlQuery global_query(
			"CREATE TABLE IF NOT EXISTS config (id INTEGER PRIMARY KEY AUTOINCREMENT, active_codecs TEXT)");
	if (!global_query.isActive())
		qWarning() << "KTelephoneManager::bootstrapDatabase - ERROR: " << global_query.lastError().text();

	QSqlQuery insert_global_query;
	insert_global_query.prepare(
			"INSERT INTO config (id, active_codecs) VALUES (1, :active_codecs)");
	insert_global_query.bindValue(":active_codecs", "PCMU/8000/1,PCMA/8000/1,opus/48000/2");

	if (!insert_global_query.exec()) {
		qWarning() << "KTelephoneManager::bootstrapDatabase - ERROR: " << insert_global_query.lastError().text();
		return;
	}
}

void KTelephoneManager::loadFromDatabase() {
	QSqlQuery query;
	query.prepare("SELECT id, description, name, domain, username, password, active, should_register_startup, should_subscribe_presence, should_publish_presence, should_use_blf, should_disable_ringback_tone, custom_ringtone, transport, subscription_expiry_delay, keep_alive_expiry_delay, registration_expiry_delay, use_stun, stun_server FROM telephones;");

	if (!query.exec()) {
		qWarning() << "KTelephoneManager::loadFromDatabase - ERROR: " << query.lastError().text();
		return;
	}

	while (query.next()) {
		Telephone_t telephone{
				query.value(0).toString(),
				query.value(1).toString(),
				query.value(2).toString(),
				query.value(3).toString(),
				query.value(4).toString(),
				query.value(5).toString(),
				query.value(6).toInt(),

				query.value(7).toInt(),
				query.value(8).toInt(),
				query.value(9).toInt(),
				query.value(10).toInt(),
				query.value(11).toInt(),
				query.value(12).toString(),
				query.value(13).toInt(),

				query.value(14).toInt(),
				query.value(15).toInt(),
				query.value(16).toInt(),
				query.value(17).toInt(),
				query.value(18).toString()
		};

		this->newKTelephone(telephone);
	}

	QSqlQuery global_query;
	global_query.prepare("SELECT active_codecs FROM config;");

	if (!global_query.exec()) {
		qWarning() << "KTelephoneManager::loadFromDatabase - ERROR: " << global_query.lastError().text();
		return;
	}

	while (global_query.next()) {
		global.active_codecs = global_query.value(0).toString();
	}
}

Globals_t KTelephoneManager::getGlobal() {
	return global;
}

void KTelephoneManager::updateGlobals(const Globals_t& _global) {
	QSqlQuery update_global_query;
	update_global_query.prepare(
			"UPDATE config SET active_codecs=:active_codecs WHERE id=1");
	update_global_query.bindValue(":active_codecs", _global.active_codecs);

	if (!update_global_query.exec()) {
		qWarning() << "KTelephoneManager::updateGlobals - ERROR: " << update_global_query.lastError().text();
		return;
	}

	global.active_codecs = _global.active_codecs;
}

bool KTelephoneManager::hasActiveAccounts() {
	QSqlQuery query;
	query.prepare("SELECT COUNT(id) FROM telephones WHERE active = 1;");

	if (!query.exec()) {
		qWarning() << "KTelephoneManager::hasActiveAccounts - ERROR: " << query.lastError().text();
		return false;
	}

	query.first();
	return query.value(0).toInt() > 0;
}

void KTelephoneManager::unloadKTelephones() {
	foreach(QString item, telephones.keys()) {
		mUAManager->removeUserAgent(item);
	}
}

void KTelephoneManager::saveTelephone(Telephone_t *telephone) {
	QSqlQuery query;
	query.prepare(
			"INSERT INTO telephones (description, name, domain, username, password, active, should_register_startup, should_subscribe_presence, should_publish_presence, should_use_blf, should_disable_ringback_tone, custom_ringtone, transport, subscription_expiry_delay, keep_alive_expiry_delay, registration_expiry_delay, use_stun, stun_server) VALUES (:description, :name, :domain, :username, :password, :active, :should_register_startup, :should_subscribe_presence, :should_publish_presence, :should_use_blf, :should_disable_ringback_tone, :custom_ringtone, :transport, :subscription_expiry_delay, :keep_alive_expiry_delay, :registration_expiry_delay, :use_stun, :stun_server)");
	query.bindValue(":description", telephone->description);
	query.bindValue(":name", telephone->name);
	query.bindValue(":domain", telephone->domain);
	query.bindValue(":username", telephone->username);
	query.bindValue(":password", telephone->password);
	query.bindValue(":active", telephone->active);
	query.bindValue(":should_register_startup", telephone->should_register_startup);
	query.bindValue(":should_subscribe_presence", telephone->should_subscribe_presence);
	query.bindValue(":should_publish_presence", telephone->should_publish_presence);
	query.bindValue(":should_use_blf", telephone->should_use_blf);
	query.bindValue(":should_disable_ringback_tone", telephone->should_disable_ringback_tone);
	query.bindValue(":custom_ringtone", telephone->custom_ringtone);
	query.bindValue(":transport", telephone->transport);
	query.bindValue(":subscription_expiry_delay", telephone->subscription_expiry_delay);
	query.bindValue(":keep_alive_expiry_delay", telephone->keep_alive_expiry_delay);
	query.bindValue(":registration_expiry_delay", telephone->registration_expiry_delay);
	query.bindValue(":use_stun", telephone->use_stun);
	query.bindValue(":stun_server", telephone->stun_server);

	if (!query.exec()) {
		qWarning() << "KTelephoneManager::saveTelephone - ERROR: " << query.lastError().text();
		return;
	}

	if (query.lastInsertId().canConvert(QMetaType::QString)) {
		telephone->id = query.lastInsertId().toString();
	}
}

void KTelephoneManager::updateTelephone(Telephone_t *telephone) {
	if (telephone->id.isEmpty() || telephone->id.isNull()) {
		qWarning() << "No id provided on update.";
		return;
	}

	QSqlQuery query;
	query.prepare(
			"UPDATE telephones SET description=:description, name=:name, domain=:domain, username=:username, password=:password, active=:active, should_register_startup=:should_register_startup, should_subscribe_presence=:should_subscribe_presence, should_publish_presence=:should_publish_presence, should_use_blf=:should_use_blf, should_disable_ringback_tone=:should_disable_ringback_tone, custom_ringtone=:custom_ringtone, transport=:transport, subscription_expiry_delay=:subscription_expiry_delay, keep_alive_expiry_delay=:keep_alive_expiry_delay, registration_expiry_delay=:registration_expiry_delay, use_stun=:use_stun, stun_server=:stun_server WHERE id = :id");
	query.bindValue(":description", telephone->description);
	query.bindValue(":name", telephone->name);
	query.bindValue(":domain", telephone->domain);
	query.bindValue(":username", telephone->username);
	query.bindValue(":password", telephone->password);
	query.bindValue(":active", telephone->active);
	query.bindValue(":should_register_startup", telephone->should_register_startup);
	query.bindValue(":should_subscribe_presence", telephone->should_subscribe_presence);
	query.bindValue(":should_publish_presence", telephone->should_publish_presence);
	query.bindValue(":should_use_blf", telephone->should_use_blf);
	query.bindValue(":should_disable_ringback_tone", telephone->should_disable_ringback_tone);
	query.bindValue(":custom_ringtone", telephone->custom_ringtone);
	query.bindValue(":transport", telephone->transport);
	query.bindValue(":subscription_expiry_delay", telephone->subscription_expiry_delay);
	query.bindValue(":keep_alive_expiry_delay", telephone->keep_alive_expiry_delay);
	query.bindValue(":registration_expiry_delay", telephone->registration_expiry_delay);
	query.bindValue(":use_stun", telephone->use_stun);
	query.bindValue(":stun_server", telephone->stun_server);
	query.bindValue(":id", telephone->id);

	qDebug() << telephone->id;

	if (!query.exec()) {
		qWarning() << "KTelephoneManager::updateTelephone - ERROR: " << query.lastError().text();
		return;
	}
}

void KTelephoneManager::deleteTelephone(Telephone_t *telephone) {
	if (telephone->id.isEmpty() || telephone->id.isNull()) {
		qWarning() << "No id provided on delete.";
		return;
	}

	QSqlQuery query;
	query.prepare("DELETE FROM telephones WHERE id = :id");
	query.bindValue(":id", telephone->id);

	if (!query.exec()) {
		qWarning() << "KTelephoneManager::deleteTelephone - ERROR: " << query.lastError().text();
		return;
	}
}


QStringList KTelephoneManager::getAvailableCodecs() {
	QStringList _availableCodecs;
	const auto _codecEnum = Endpoint::instance().codecEnum2();
	std::for_each(_codecEnum.begin(), _codecEnum.end(), [&](const auto& item){
		_availableCodecs.append(QString::fromStdString(item.codecId));
	});

	QStringListIterator i(this->getActiveCodecs());
	while(i.hasNext()) {
		_availableCodecs.removeAll(i.next());
	}

	return _availableCodecs;
}

QStringList KTelephoneManager::getActiveCodecs() {
	QStringList active_codecs = global.active_codecs.split(",");
	return active_codecs;
}


void KTelephoneManager::open() {
	this->bootstrap();
}

void KTelephoneManager::openPreferences(void) {
	if (mPreferences) {
		mPreferences->show();
		mPreferences->activateWindow();
		mPreferences->raise();
		mPreferences->setFocus();
		return;
	}

	mPreferences = new KTelephonePreferences(this);
	mPreferences->setManager(this);
	mPreferences->show();
}

void KTelephoneManager::openAbout() {
	if (mAbout) {
		mAbout->show();
		mAbout->activateWindow();
		mAbout->raise();
		mAbout->setFocus();
		return;
	}

	mAbout = new KTelephoneAbout(this);
	mAbout->show();
}

void KTelephoneManager::startGuide() {
	mGuide = new KTelephoneGuide();
	mGuide->setManager(this);
	mGuide->show();
}
