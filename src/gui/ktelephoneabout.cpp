#include "ktelephoneabout.hpp"

KTelephoneAbout::KTelephoneAbout(QWidget *parent) :
		QDialog(),
		ui(new Ui::about) {
	ui->setupUi(this);
}

KTelephoneAbout::~KTelephoneAbout() {
	delete ui;
}