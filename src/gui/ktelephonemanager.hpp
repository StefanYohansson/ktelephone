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
};

typedef struct Telephone Telephone_t;

#include "uamanager.hpp"

class UserAgentManager;

class KTelephoneManager : public QMainWindow {
Q_OBJECT
public:
    explicit KTelephoneManager(QWidget *parent = 0);

    ~KTelephoneManager();

    void newKTelephone(Telephone_t);

    void updateKTelephone(QString, Telephone_t);

    void saveTelephone(Telephone_t *);

    void updateTelephone(Telephone_t);

    void deleteTelephone(Telephone_t);

    void removeKTelephone(Telephone_t);

    QHash<QString, KTelephone *> getTelephones();

    UserAgentManager *getUserAgentManager();

    void openPreferences();

	void openAbout();

private:
    void bootstrap();

    QSystemTrayIcon *trayIcon = NULL;
    QMenu *trayIconMenu = NULL;

private slots:

    void open();

protected:
    QHash<QString, KTelephone *> telephones;
    KTelephone *mTelephone = NULL;
    KTelephoneGuide *mGuide = NULL;
    KTelephonePreferences *mPreferences = NULL;
    KTelephoneAbout *mAbout = NULL;
    UserAgentManager *mUAManager = NULL;

    void startGuide();

    void connectDatabase();

    void bootstrapDatabase();

    void loadFromDatabase();

    void unloadKTelephones();
};

#endif // KTELEPHONEMANAGER_H
