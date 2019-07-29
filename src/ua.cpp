#include "ua.h"

#include <QDebug>

void UserAgent::onRegState(OnRegStateParam &prm)
{
  AccountInfo ai = getInfo();
  ai.regIsActive ? this->mTelephone->changeRegistrationStatus(true) : this->mTelephone->changeRegistrationStatus(false);
  qDebug() << (ai.regIsActive? "*** Register:" : "*** Unregister:")
            << " code=" << prm.code;
}

void UserAgent::onIncomingCall(OnIncomingCallParam &iprm)
{
  qDebug() << "receiving a call";
  Call *call = new MyCall(*this, iprm.callId);

  // Just hangup for now
  CallOpParam op;
  op.statusCode = PJSIP_SC_BUSY_HERE;
  call->hangup(op);
  qDebug() << "hangup the call";

  // And delete the call
  delete call;
}

void UserAgent::setInstance(KTelephone* telephone)
{
  this->mTelephone = telephone;
}
