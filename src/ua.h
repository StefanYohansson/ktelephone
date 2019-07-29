#ifndef KTELEPHONEUA_H
#define KTELEPHONEUA_H

#include <pjsua2.hpp>
#include "ktelephone.h"
#include "mycall.h"

using namespace pj;

class UserAgent : public Account
{
 public:
  virtual void onRegState(OnRegStateParam &prm);
  virtual void onIncomingCall(OnIncomingCallParam &iprm);
  void setInstance(KTelephone* telephone);

protected:
  KTelephone *mTelephone = NULL;
};

#endif // KTELEPHONEUA_H
