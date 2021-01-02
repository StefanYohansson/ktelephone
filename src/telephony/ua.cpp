#include "ua.hpp"

#include <QDebug>

UserAgent::UserAgent(QObject *parent)
        : QObject(parent) {
}

UserAgent::~UserAgent() {
}

void UserAgent::onRegState(OnRegStateParam &prm) {
    AccountInfo ai = getInfo();
    ai.regIsActive ? this->mTelephone->changeRegistrationStatus(true) : this->mTelephone->changeRegistrationStatus(
            false);
    qDebug() << (ai.regIsActive ? "*** Register:" : "*** Unregister:")
             << " code=" << prm.code;
}

void UserAgent::onIncomingCall(OnIncomingCallParam &iprm) {
    MyCall *call = new MyCall(*this, iprm.callId);
    emit sNewCall(call);
    // mCalls.insert(QString::fromStdString(std::to_string(iprm.callId)), call);
}

void UserAgent::setInstance(KTelephone *telephone) {
    this->mTelephone = telephone;
    QObject::connect(this, SIGNAL(sNewCall(MyCall * )), this->mTelephone, SLOT(actionInboundCall(MyCall * )));
}
