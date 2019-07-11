#include "ua.h"

#include <QDebug>

void UserAgent::onRegState(OnRegStateParam &prm)
{
  AccountInfo ai = getInfo();
  ai.regIsActive ? this->mTelephone->changeRegistrationStatus(true) : this->mTelephone->changeRegistrationStatus(false);
  qDebug() << (ai.regIsActive? "*** Register:" : "*** Unregister:")
            << " code=" << prm.code;
}

void UserAgent::setInstance(KTelephone* telephone)
{
  this->mTelephone = telephone;
}
