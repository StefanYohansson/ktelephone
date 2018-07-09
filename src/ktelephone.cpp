#include "ktelephone.h"
#include "ui_ktelephone.h"

ktelephone::ktelephone(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ktelephone)
{
    ui->setupUi(this);
}

ktelephone::~ktelephone()
{
    delete ui;
}
