#include "call.h"

#include "ui_call.h"
#include <QDebug>

KTelephoneCall::KTelephoneCall(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::call)
{
  ui->setupUi(this);
}

KTelephoneCall::~KTelephoneCall()
{
  delete ui;
}
