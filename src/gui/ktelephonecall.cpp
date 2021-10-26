#include "ktelephonecall.hpp"

#include "ui_call.h"
#include <QDebug>

KTelephoneCall::KTelephoneCall(KTelephone *parent, QString direction, QString username) :
        QDialog(),
        ui(new Ui::call) {
    ui->setupUi(this);

    this->callDirection = direction;
    this->calleeUsername = username;

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_A), this,
                  SLOT(actionAnswer()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this,
                  SLOT(actionHangup()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_H), this,
                  SLOT(actionHold()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_M), this,
                  SLOT(actionMute()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_T), this,
                  SLOT(actionTransfer()));

    connect(ui->answerButton,
            SIGNAL(clicked()), this,
            SLOT(actionAnswer()));
    connect(ui->cancelButton,
            SIGNAL(clicked()), this,
            SLOT(actionHangup()));

    connect(ui->muteButton,
            SIGNAL(clicked()), this,
            SLOT(actionMute()));
    connect(ui->holdButton,
            SIGNAL(clicked()), this,
            SLOT(actionHold()));
    connect(ui->transferButton,
            SIGNAL(clicked()), this,
            SLOT(actionTransfer()));
    connect(ui->dtmfInput,
            SIGNAL(textEdited(QString)), this,
            SLOT(actionDtmf(QString)));

    ui->dtmfInput->hide();
    ui->dtmfInput->hide();
    ui->callAction->hide();

    if (callDirection == "outbound") {
        ui->answerButton->deleteLater();
    }
}

KTelephoneCall::~KTelephoneCall() {
    delete ui;
}

void KTelephoneCall::setInstance(MyCall *telephoneCall) {
    this->mCall = telephoneCall;
    CallInfo ci = this->mCall->getInfo();
    QString whoLabel;
    if (this->callDirection == "outbound") {
        whoLabel.append("To: ");
        whoLabel.append(QString::fromStdString(ci.remoteUri));
        QSound *outboundAudio = new QSound("data/sounds/outbound-ring.wav");
        outboundAudio->setLoops(QSound::Infinite);
        outboundAudio->play();
        this->outboundAudio = outboundAudio;
    } else {
        // Ringing
        whoLabel.append("From: ");
        whoLabel.append(QString::fromStdString(ci.localContact));
        QSound *inboundAudio = new QSound("data/sounds/inbound-ring.wav");
        inboundAudio->setLoops(QSound::Infinite);
        inboundAudio->play();
        this->inboundAudio = inboundAudio;
    }
    ui->whoLabel->setText(whoLabel);
}

void KTelephoneCall::callDestroy() {
    if (this->mCall) {
        delete this->mCall;
    }
    this->close();
}

void KTelephoneCall::closeEvent(QCloseEvent* event) {
    if (this->outboundAudio) {
        this->outboundAudio->stop();
    }
    if (this->inboundAudio) {
        this->inboundAudio->stop();
    }
}

void KTelephoneCall::callbackAnswer() {
    ui->dtmfInput->show();
    ui->callAction->show();
    this->answered = true;
    if (this->outboundAudio) {
        this->outboundAudio->stop();
    }
}

void KTelephoneCall::actionAnswer() {
    ui->answerButton->setEnabled(false);
    this->mCall->doAnswer();

    ui->dtmfInput->show();
    ui->callAction->show();
    if (this->inboundAudio) {
        this->inboundAudio->stop();
    }
}

void KTelephoneCall::actionHangup() {
    this->mCall->doHangup();
    this->close();
}

void KTelephoneCall::actionHold() {
    if (!this->hold) {
        ui->holdButton->setText("Release");
    } else {
        ui->holdButton->setText("Hold");
    }
    this->mCall->doHold(this->hold);
    this->hold = !this->hold;
}

void KTelephoneCall::actionMute() {
    if (!this->mute) {
        ui->muteButton->setText("Unmute");
    } else {
        ui->muteButton->setText("Mute");
    }
    this->mCall->doMute(this->mute);
    this->mute = !this->mute;
}

void KTelephoneCall::actionDtmf(QString text) {
    if (!this->answered
        || this->previousDtmf.length() >= text.length()) {
        this->previousDtmf = text;
        return;
    }
    this->mCall->doDtmf(text.right(1));
    this->previousDtmf = text;
}

void KTelephoneCall::actionTransfer() {
    const QString destination = ui->dtmfInput->text();
    if (destination.isEmpty()) {
        return;
    }
    this->mCall->doTransfer(destination, this->calleeUsername);
}
