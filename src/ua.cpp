#include "ua.h"

#include <QDebug>

void UserAgent::onRegState(OnRegStateParam &prm)
{
  AccountInfo ai = getInfo();
  qDebug() << (ai.regIsActive? "*** Register:" : "*** Unregister:")
            << " code=" << prm.code;
}
