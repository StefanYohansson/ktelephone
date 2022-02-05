#include "ktelephonetransfercall.hpp"
#include <QDebug>

KTelphoneTransferCall::KTelphoneTransferCall(std::function<void(QString)> onTransferClick) :
		QDialog(),
		ui(new Ui::transfercall) {
	ui->setupUi(this);
	ui->destinationNumberInput->setPlaceholderText(tr("Destination Number"));
	ui->transferButton->setDefault(true);

	this->onTransferClick = onTransferClick;

	connect(ui->transferButton,
			SIGNAL(clicked()), this,
			SLOT(actionTransfer()));

	connect(ui->cancelButton,
			SIGNAL(clicked()), this,
			SLOT(actionCancel()));

	connect(this,
			SIGNAL(rejected()), this,
			SLOT(resetState()));
}

KTelphoneTransferCall::~KTelphoneTransferCall() {
	delete ui;
}

void KTelphoneTransferCall::actionTransfer() {
	this->onTransferClick(ui->destinationNumberInput->text());
}

void KTelphoneTransferCall::actionCancel() {
	this->close();
}

void KTelphoneTransferCall::resetState() {
	ui->destinationNumberInput->clear();
	ui->destinationNumberInput->setFocus();
}

void KTelphoneTransferCall::closeEvent(QCloseEvent *event) {
	resetState();
}
