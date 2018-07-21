#ifndef KTELEPHONEUAMANAGER_H
#define KTELEPHONEUAMANAGER_H

#include <pjsua2.hpp>
#include <QHash>

#include "ua.h"
#include "ktelephonemanager.h"

using namespace pj;

class UserAgentManager
{
 public:
  int bootstrapManager();
  AccountConfig getAccountConfig(Telephone_t mTelephone);
  void newUserAgent(QString domain, AccountConfig acfg);
  void removeUserAgent(QString);

 protected:
  QHash<QString, UserAgent*> accounts;
  
};

#endif // KTELEPHONEUAMANAGER_H
