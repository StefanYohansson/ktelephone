#ifndef CALL_H
#define CALL_H

#include <QDialog>
#include <QSound>
#include <QShortcut>

class MyCall;

#include "ktelephone.hpp"

namespace Ui {
	class call;
}

class KTelephoneCall : public QDialog {
Q_OBJECT

public:
	explicit KTelephoneCall(KTelephone *parent = 0, QString direction = "inbound", QString username = "", int disableRingback = 0, QString customRingtone = "");

	~KTelephoneCall();

	void setInstance(MyCall *telephoneCall);

	void callDestroy();

	void callbackAnswer();

private:
	Ui::call *ui;
	MyCall *mCall = NULL;
	QString callDirection;
	QString calleeUsername;
	QString customRingtone;
	QSound *inboundAudio = NULL;
	QSound *outboundAudio = NULL;
	bool answered = false;
	bool hold = false;
	bool mute = false;
	bool disableRingback = false;
	QString previousDtmf;

public slots:

	void actionHangup();

	void actionAnswer();

	void actionMute();

	void actionHold();

	void actionDtmf(QString);

	void actionTransfer();

};


#endif // CALL_H
