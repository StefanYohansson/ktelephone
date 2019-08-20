#include "ktelephonecall.h"

#include "ui_call.h"
#include <QDebug>

KTelephoneCall::KTelephoneCall(KTelephone *parent, QString direction) :
    QDialog(),
    ui(new Ui::call)
{
  ui->setupUi(this);

  callDirection = direction;

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

  if (callDirection == "outbound") {
    ui->answerButton->deleteLater();
  }
}

KTelephoneCall::~KTelephoneCall()
{
  delete ui;
}

void KTelephoneCall::setInstance(MyCall* telephoneCall)
{
  this->mCall = telephoneCall;
  CallInfo ci = this->mCall->getInfo();
  QString whoLabel;
  if (callDirection == "outbound") {
    whoLabel.append("To: ");
    whoLabel.append(QString::fromStdString(ci.remoteUri));
  } else {
    whoLabel.append("From: ");
    whoLabel.append(QString::fromStdString(ci.localContact));
  }
  ui->whoLabel->setText(whoLabel);
}

void KTelephoneCall::callDestroy()
{
  if (this->mCall) {
    delete this->mCall;
  }
  this->close();
}

void KTelephoneCall::actionAnswer()
{
  ui->answerButton->setEnabled(false);
  this->mCall->doAnswer();
}

void KTelephoneCall::actionHangup()
{
  this->mCall->doHangup();
  this->close();
}

void KTelephoneCall::actionHold()
{
  if (!this->hold) {
    ui->holdButton->setText("Release");
  } else {
    ui->holdButton->setText("Hold");
  }
  this->mCall->doHold(this->hold);
  this->hold = !this->hold;
}

void KTelephoneCall::actionMute()
{
  if (!this->mute) {
    ui->muteButton->setText("Unmute");
  } else {
    ui->muteButton->setText("Mute");
  }
  this->mCall->doMute(this->mute);
  this->mute = !this->mute;
}

void KTelephoneCall::actionTransfer()
{

}
