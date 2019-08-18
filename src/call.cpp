#include "call.h"

#include "ui_call.h"
#include <QDebug>

KTelephoneCall::KTelephoneCall(KTelephone *parent) :
    QDialog(),
    ui(new Ui::call)
{
  ui->setupUi(this);

  connect(ui->answerButton,
          SIGNAL(clicked()), this,
          SLOT(actionAnswer()));
  connect(ui->cancelButton,
          SIGNAL(clicked()), this,
          SLOT(actionHangup()));
}

KTelephoneCall::~KTelephoneCall()
{
  delete ui;
}

void KTelephoneCall::setInstance(MyCall* telephoneCall)
{
  this->mCall = telephoneCall;
  CallInfo ci = this->mCall->getInfo();
  ui->whoLabel->setText(QString::fromStdString(ci.localContact));
}

void KTelephoneCall::callDestroy()
{
  if (this->mCall) {
    delete this->mCall;
  }
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
