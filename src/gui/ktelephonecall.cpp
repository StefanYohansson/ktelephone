#include "ktelephonecall.hpp"
#include "ktelephonetransfercall.hpp"
#include "ktelephoneutil.hpp"

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
                  SLOT(openTransferCallDialog()));

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
            SLOT(openTransferCallDialog()));
    connect(ui->dtmfInput,
            SIGNAL(textEdited(QString)), this,
            SLOT(actionDtmf(QString)));

    connect(this,
            SIGNAL(rejected()), this,
            SLOT(onWindowClose()));

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
    this->contact = getNumberFromURI(QString::fromStdString(ci.remoteUri));
    if (this->callDirection == "outbound") {
        whoLabel.append("To: ");
        whoLabel.append(this->contact);
        QSound *outboundAudio = new QSound("data/sounds/outbound-ring.wav");
        outboundAudio->setLoops(QSound::Infinite);
        outboundAudio->play();
        this->outboundAudio = outboundAudio;
    } else {
        // Ringing
        whoLabel.append("From: ");
        whoLabel.append(this->contact);
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
        this->mCall = NULL;
    }
    this->close();
}

void KTelephoneCall::closeEvent(QCloseEvent* event) {
    this->onWindowClose();
}

void KTelephoneCall::onWindowClose() {
    if (this->outboundAudio) {
        this->outboundAudio->stop();
    }
    if (this->inboundAudio) {
        this->inboundAudio->stop();
    }
    if (this->mCall) {
        this->mCall->doHangup();
        delete this->mCall;
    }
    if (this->transferCall) {
        this->transferCall->close();
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

void KTelephoneCall::actionTransfer(QString destinationNumber) {
    if (destinationNumber.isEmpty() || destinationNumber.isNull()) {
        return;
    }
    this->mCall->doTransfer(destinationNumber, this->calleeUsername);
    this->transferCall->close();
}

void KTelephoneCall::openTransferCallDialog() {
    if (!this->transferCall) {
        const auto onClickTransfer = std::bind(&KTelephoneCall::actionTransfer, this, std::placeholders::_1);
        this->transferCall = new KTelphoneTransferCall(onClickTransfer);
        this->transferCall->setWindowTitle(contact);
    }
    this->transferCall->show();
}
