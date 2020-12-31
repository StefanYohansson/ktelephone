#ifndef KTELEPHONEUAMANAGER_H
#define KTELEPHONEUAMANAGER_H

#include <QObject>
#include <pjsua2.hpp>
#include <QHash>

#include "ua.hpp"
#include "mycall.hpp"
#include "ktelephonemanager.hpp"

using namespace pj;

class UserAgent;

class KTelephoneManager;

class UserAgentManager : public QObject {
Q_OBJECT
public:
    explicit UserAgentManager(QObject *parent = 0);

    ~UserAgentManager();

    AccountConfig getAccountConfig(Telephone_t mTelephone);

    void newUserAgent(KTelephone *telephone, QString username, AccountConfig acfg);

    void removeUserAgent(QString domain);

    void setRegister(QString domain, bool status);

    MyCall *placeCall(const QString &username, const QString &dest);

    void start(int port);

protected:
    QHash<QString, UserAgent *> mAccounts;
    Endpoint *ep;
    EpConfig ep_cfg;
    TransportConfig tcfg;

};

#endif // KTELEPHONEUAMANAGER_H
