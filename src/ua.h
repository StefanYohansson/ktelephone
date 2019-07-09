#ifndef KTELEPHONEUA_H
#define KTELEPHONEUA_H

#include <pjsua2.hpp>
#include "ktelephone.h"

using namespace pj;

class UserAgent : public Account
{
 public:
  virtual void onRegState(OnRegStateParam &prm);
  void setInstance(KTelephone* telephone);

protected:
  KTelephone *mTelephone = NULL;

};

#endif // KTELEPHONEUA_H
