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
