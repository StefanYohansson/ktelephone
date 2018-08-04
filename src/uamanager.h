#ifndef KTELEPHONEUAMANAGER_H
#define KTELEPHONEUAMANAGER_H

#include <pjsua2.hpp>
#include <QHash>

#include "ua.h"
#include "ktelephonemanager.h"

using namespace pj;

class KTelephoneManager;

class UserAgentManager
{
 public:
  UserAgentManager();
  ~UserAgentManager();
  AccountConfig getAccountConfig(Telephone_t mTelephone);
  void newUserAgent(QString domain, AccountConfig acfg);
  void removeUserAgent(QString domain);

 protected:
  QHash<QString, UserAgent*> mAccounts;
  Endpoint *ep;
  EpConfig ep_cfg;
  TransportConfig tcfg;
  
};

#endif // KTELEPHONEUAMANAGER_H
