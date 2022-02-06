#ifndef KTELEPHONEMANAGER_H
#define KTELEPHONEMANAGER_H

#include <QApplication>
#include <QMainWindow>
#include <QObject>
#include <QHash>
#include <QSqlDatabase>
#include <QSystemTrayIcon>
#include <QMenu>

class KTelephone;

class KTelephoneGuide;

class KTelephoneCall;

class KTelephonePreferences;

class KTelephoneAbout;

struct Telephone {
	QString id;
	QString description;
	QString name;
	QString domain;
	QString username;
	QString password;
	int active;

	/* prefs */
	int should_register_startup;
	int should_subscribe_presence;
	int should_publish_presence;
	int should_use_blf;
	int should_disable_ringback_tone;
	QString custom_ringtone;

	int transport;
	int subscription_expiry_delay;
	int keep_alive_expiry_delay;
	int registration_expiry_delay;
	int use_stun;
	QString stun_server;
};

struct Globals {
		QString active_codecs;
};

typedef struct Globals Globals_t;
typedef struct Telephone Telephone_t;

#include "uamanager.hpp"

class UserAgentManager;

class KTelephoneManager : public QMainWindow {
Q_OBJECT
public:
	explicit KTelephoneManager(QWidget *parent = 0);

	~KTelephoneManager();

	void newKTelephone(Telephone_t);

	void updateKTelephone(QString, Telephone_t *);

	void saveTelephone(Telephone_t *);

	void updateTelephone(Telephone_t *);

	void deleteTelephone(Telephone_t *);

	void removeKTelephone(Telephone_t *);

	QHash<QString, KTelephone *> getTelephones();

	UserAgentManager *getUserAgentManager();

	Globals_t getGlobal();

	void updateGlobals(const Globals_t& global);

	QStringList getAvailableCodecs();

	QStringList getActiveCodecs();

	void openPreferences();

	void openAbout();

private:
	void bootstrap();

	void bootstrapGlobal();

	QSystemTrayIcon *trayIcon = NULL;
	QMenu *trayIconMenu = NULL;

private slots:

	void open();

	void openPreferencesSlot();

protected:
	QHash<QString, KTelephone *> telephones;
	KTelephone *mTelephone = NULL;
	KTelephoneGuide *mGuide = NULL;
	KTelephonePreferences *mPreferences = NULL;
	KTelephoneAbout *mAbout = NULL;
	UserAgentManager *mUAManager = NULL;
	Globals_t global;


	void startGuide();

	void connectDatabase();

	static void bootstrapDatabase();

	void loadFromDatabase();

	void unloadKTelephones();

	bool hasActiveAccounts();
};

#endif // KTELEPHONEMANAGER_H
