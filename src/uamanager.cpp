#include "uamanager.h"

#include <QDebug>
#include <QString>

int UserAgentManager::bootstrapManager()
{
  Endpoint ep;

  ep.libCreate();

  // Initialize endpoint
  EpConfig ep_cfg;
  ep.libInit( ep_cfg );

  // Create SIP transport. Error handling sample is shown
  TransportConfig tcfg;
  tcfg.port = 5060;
  try {
    ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
  } catch (Error &err) {
    return 1;
  }

  // Start the library (worker threads etc)
  ep.libStart();
  qDebug() << "*** PJSUA2 STARTED ***";

  return 0;
}

AccountConfig UserAgentManager::getAccountConfig(Telephone_t mTelephone)
{
  // Configure an AccountConfig
  AccountConfig acfg;
  QString sip = "sip:";
  acfg.idUri = (sip.append(mTelephone.domain)).toUtf8().constData();
  acfg.regConfig.registrarUri = (sip.append(mTelephone.domain.split('@').takeLast())).toUtf8().constData();

  AuthCredInfo cred("digest", "*", mTelephone.username.toUtf8().constData(), 0, mTelephone.password.toUtf8().constData());
  acfg.sipConfig.authCreds.push_back( cred );
  return acfg;
}
