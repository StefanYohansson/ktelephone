#ifndef KTELEPHONEUAMANAGER_H
#define KTELEPHONEUAMANAGER_H

#include <pjsua2.hpp>
#include <QHash>

#include "ua.h"
#include "ktelephonemanager.h"

using namespace pj;

class UserAgent;
class KTelephoneManager;

class UserAgentManager
{
 public:
  UserAgentManager();
  ~UserAgentManager();
  AccountConfig getAccountConfig(Telephone_t mTelephone);
  void newUserAgent(KTelephone* telephone, QString username, AccountConfig acfg);
  void removeUserAgent(QString domain);
  void setRegister(QString domain, bool status);
  void start(int port);

 protected:
  QHash<QString, UserAgent*> mAccounts;
  Endpoint *ep;
  EpConfig ep_cfg;
  TransportConfig tcfg;

};

#endif // KTELEPHONEUAMANAGER_H
