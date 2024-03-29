#include "uamanager.hpp"

#include <QDebug>
#include <QString>

UserAgentManager::UserAgentManager(QObject *parent)
		: QObject(parent) {
	Endpoint *ep = new Endpoint;
	ep->libCreate();

	// Initialize endpoint
	ep->libInit(ep_cfg);

	this->start(5090);

	auto _parent = (KTelephoneManager *) parent;
	auto activeCodecs = _parent->getGlobal().active_codecs.split(",");
	auto disabledCodecs = _parent->getAvailableCodecs();

	int priority = 100;
	int counter = 0;
	foreach(auto codec, activeCodecs) {
		try {
			ep->codecSetPriority(codec.toStdString(), priority + counter);
		} catch (Error &err) {
			qDebug() << "Cannot import " << codec;
		}
		counter++;
	}

	foreach(auto codec, disabledCodecs) {
		try {
			ep->codecSetPriority(codec.toStdString(), 0);
		} catch (Error &err) {
			qDebug() << "Cannot disable " << codec;
		}
	}
}

UserAgentManager::~UserAgentManager() {
	//  ep->libDestroy();
	//  delete ep;
}

void UserAgentManager::start(int port) {
	try {
		// Create SIP transport. Error handling sample is shown
		tcfg.port = port;

		ep->transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
		ep->transportCreate(PJSIP_TRANSPORT_TCP, tcfg);
		ep->transportCreate(PJSIP_TRANSPORT_TLS, tcfg);

		// Start the library (worker threads etc)
		ep->libStart();

		qDebug() << "*** PJSUA2 STARTED ***";
	} catch (Error &err) {
		qDebug() << "Error starting UserAgentManager";
		this->start(port + 1);
		return;
	}
}

AccountConfig UserAgentManager::getAccountConfig(Telephone_t *mTelephone) {
	// Configure an AccountConfig
	AccountConfig acfg;
	QString sip = "sip:";
	QString sipUri = QString();
	acfg.idUri = sipUri.append(sip).append(mTelephone->username).toStdString();
	QString username = QString();
	QString domain = QString();
	domain.append(sip);
	if (!mTelephone->username.split('@').isEmpty()) {
		domain.append(mTelephone->username.split('@').takeLast());
		username.append(mTelephone->username.split('@').takeFirst());
	} else {
		username.append(mTelephone->username);
	}

	if (mTelephone->transport == PJSIP_TRANSPORT_TCP) {
		domain.append(";transport=tcp");
	} else if (mTelephone->transport == PJSIP_TRANSPORT_TLS) {
		domain.append(";transport=tls");
	}

	acfg.regConfig.registrarUri = domain.toStdString();
	acfg.regConfig.registerOnAdd = mTelephone->should_register_startup;
	acfg.regConfig.timeoutSec = mTelephone->registration_expiry_delay;
	acfg.natConfig.udpKaIntervalSec = mTelephone->keep_alive_expiry_delay;

	AuthCredInfo cred("digest", "*", username.toStdString(), 0, mTelephone->password.toStdString());
	acfg.sipConfig.authCreds.push_back(cred);
	return acfg;
}

void UserAgentManager::newUserAgent(KTelephone *telephone, QString username, AccountConfig acfg) {
	UserAgent *acc = new UserAgent;
	try {
		acc->setInstance(telephone);
		acc->create(acfg);
		mAccounts[username] = acc;
	} catch (Error &err) {
		qDebug() << "Account creation error";
		delete acc;
	}
}

void UserAgentManager::removeUserAgent(QString username) {
	if (mAccounts.contains(username)) {
		// delete Account in order to unregister
		delete mAccounts.value(username);
		mAccounts.remove(username);
	}
}

void UserAgentManager::setRegister(QString username, bool status) {
	if (mAccounts[username]) {
		try {
			mAccounts[username]->setRegistration(status);
		} catch (Error &err) {
			mAccounts[username]->dispatchUiMessage(QString::fromStdString(err.reason));
		}
	}
}

MyCall *UserAgentManager::placeCall(QString &username, const QString &dest) {
	QString sip = "sip:";
	QString sipUri = QString();
	MyCall *call = new MyCall(*mAccounts[username]);
	CallOpParam prm(true);
	sipUri.append(sip);
	sipUri.append(dest);
	if (!dest.split('@').isEmpty()) {
		sipUri.append('@');
		sipUri.append(username.split('@').takeLast());
	}
	call->makeCall(sipUri.toStdString(), prm);
	return call;
}
