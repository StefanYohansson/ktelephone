#ifndef KTELEPHONEUA_H
#define KTELEPHONEUA_H

#include <QObject>
#include <QStatusBar>
#include <pjsua2.hpp>
#include "ktelephone.hpp"
#include "mycall.hpp"

using namespace pj;

class UserAgent : public QObject, public Account {
Q_OBJECT
public:
	explicit UserAgent(QObject *parent = 0);

	~UserAgent();

	virtual void onRegState(OnRegStateParam &prm);

	virtual void onIncomingCall(OnIncomingCallParam &iprm);

	void setInstance(KTelephone *telephone);

	void dispatchUiMessage(QString message);

protected:
	KTelephone *mTelephone = NULL;
	QHash<QString, MyCall *> mCalls;

signals:

	void sNewCall(MyCall *call);
};

#endif // KTELEPHONEUA_H
