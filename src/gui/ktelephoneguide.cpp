#include "ktelephonemanager.hpp"

#include "ktelephoneguide.hpp"
#include "ui_startguide.h"
#include <QDebug>

KTelephoneGuide::KTelephoneGuide(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::guide) {
	ui->setupUi(this);

	connect(ui->buttonBox,
	        SIGNAL(accepted()), this,
	        SLOT(accept()), Qt::UniqueConnection);
	connect(ui->usernameEdit,
	        SIGNAL(textEdited(const QString &)), this,
	        SLOT(usernameChanged(const QString &)));

	ui->usernameEdit->setPlaceholderText("extension@domain");
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

KTelephoneGuide::~KTelephoneGuide() {
	delete ui;
}

void KTelephoneGuide::setManager(KTelephoneManager *manager) {
	mManager = manager;
}

void KTelephoneGuide::usernameChanged(const QString &text) {
	if (text.contains('@')) {
		ui->descriptionEdit->setText(text.split('@').takeFirst());
		ui->nameEdit->setText(text.split('@').takeFirst());
		ui->domainEdit->setText(text.split('@').takeLast());
		if (!text.split('@').takeLast().isEmpty()) {
			ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		} else {
			ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		}
	} else {
		ui->domainEdit->setText("");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
}

void KTelephoneGuide::accept() {
	if (ui->descriptionEdit->text() == '\0' ||
	    ui->nameEdit->text() == '\0' ||
	    ui->domainEdit->text() == '\0' ||
	    ui->usernameEdit->text() == '\0' ||
	    ui->passwordEdit->text() == '\0') {
		return;
	}

	Telephone_t telephone{
		.id=QString("0"),
		.description=ui->descriptionEdit->text(),
		.name=ui->nameEdit->text(),
		.domain=ui->domainEdit->text(),
		.username=ui->usernameEdit->text(),
		.password=ui->passwordEdit->text(),
		.active=1, // active

		// prefs
		.should_register_startup=1,
		.should_subscribe_presence=0,
		.should_publish_presence=0,
		.should_use_blf=0,
		.should_disable_ringback_tone=0,
		.custom_ringtone=QString(""),

		.transport=1,
		.subscription_expiry_delay=0,
		.keep_alive_expiry_delay=15,
		.registration_expiry_delay=300,
		.use_stun=0,
		.stun_server=QString("")
	};

	mManager->saveTelephone(&telephone);
	mManager->newKTelephone(telephone);
	this->close();
}
