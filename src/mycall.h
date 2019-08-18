#ifndef KTELEPHONEMYCALL_H
#define KTELEPHONEMYCALL_H

#include <pjsua2.hpp>
#include "call.h"

using namespace pj;

class KTelephoneCall;

class MyCall : public Call
{
 public:
  MyCall(Account &acc, int call_id = PJSUA_INVALID_ID)
    : Call(acc, call_id)
    {}
  // Notification when call's state has changed.
  virtual void onCallState(OnCallStateParam &prm);

  // Notification when call's media state has changed.
  virtual void onCallMediaState(OnCallMediaStateParam &prm);

  void setInstance(KTelephoneCall* telephoneCall);

  void doAnswer();

  void doHangup();

protected:
  KTelephoneCall *mCall = NULL;
};

#endif // KTELEPHONEMYCALL_H