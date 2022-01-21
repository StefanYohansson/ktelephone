#include "ktelephone.hpp"
#include "ktelephonepreferences.hpp"
#include "ui_settings.h"

#include <QDebug>

KTelephonePreferences::KTelephonePreferences(QWidget *parent) :
		QMainWindow(),
		ui(new Ui::settings) {
	ui->setupUi(this);

	connect(ui->telephonesList,
	        SIGNAL(currentItemChanged(QListWidgetItem * , QListWidgetItem * )), this,
	        SLOT(itemChanged(QListWidgetItem * , QListWidgetItem * )));
	connect(ui->addButton,
	        SIGNAL(clicked()), this,
	        SLOT(newItem()));
	connect(ui->removeButton,
	        SIGNAL(clicked()), this,
	        SLOT(removeItem()));
	connect(ui->saveButton,
	        SIGNAL(released()), this,
	        SLOT(saveChanges()));
}

KTelephonePreferences::~KTelephonePreferences() {
	delete ui;
}

void KTelephonePreferences::closeEvent(QCloseEvent *event) {
	this->hide();
	event->ignore();
}

void KTelephonePreferences::show() {
	QWidget::show();

	this->reload();
}

void KTelephonePreferences::reload() {
	currentTelephone = NULL;
	ui->telephonesList->clear();
	ui->toolBox->hide();
	QHash<QString, KTelephone *> mTelephones = mManager->getTelephones();

			foreach(QString item, mTelephones.keys()) {
			ui->telephonesList->addItem(mTelephones[item]->mTelephone.username);
		}
}

void KTelephonePreferences::setManager(KTelephoneManager *manager) {
	mManager = manager;

	this->reload();
}

void KTelephonePreferences::itemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
	QHash<QString, KTelephone *> mTelephones = mManager->getTelephones();
	if (!current || !mTelephones.contains(current->text())) {
		return;
	}

	ui->toolBox->show();

	KTelephone *item = mTelephones.value(current->text());
	const Telephone_t mTelephone = item->mTelephone;

	bool shouldEnableInputs = true;
	if (mTelephone.active) {
		shouldEnableInputs = false;
	}

	ui->descriptionEdit->setText(mTelephone.description);
	ui->descriptionEdit->setEnabled(shouldEnableInputs);
	ui->nameEdit->setText(mTelephone.name);
	ui->nameEdit->setEnabled(shouldEnableInputs);
	ui->domainEdit->setText(mTelephone.domain);
	ui->domainEdit->setEnabled(shouldEnableInputs);
	ui->usernameEdit->setText(mTelephone.username);
	ui->usernameEdit->setEnabled(shouldEnableInputs);
	ui->passwordEdit->setText(mTelephone.password);
	ui->passwordEdit->setEnabled(shouldEnableInputs);
	ui->activeCheckbox->setChecked(mTelephone.active);
	ui->registerStartupCheckbox->setChecked(mTelephone.should_register_startup);
	ui->transportCombobox->setCurrentIndex(mTelephone.transport - 1);

	currentTelephone = current;
}

void KTelephonePreferences::saveChanges() {
	if (currentTelephone == NULL) {
		return;
	}

	QHash<QString, KTelephone *> mTelephones = mManager->getTelephones();
	if (!mTelephones.contains(currentTelephone->text())) {
		return;
	}

	KTelephone *item = mTelephones.value(currentTelephone->text());
	Telephone_t mTelephone = item->mTelephone;

	const bool shouldUpdateInstance = mTelephone.active != ui->activeCheckbox->checkState();
	const QString oldUsername = mTelephone.username;
	mTelephone.description = ui->descriptionEdit->text();
	mTelephone.name = ui->nameEdit->text();
	mTelephone.username = ui->usernameEdit->text();
	mTelephone.password = ui->passwordEdit->text();
	mTelephone.domain = ui->domainEdit->text();
	mTelephone.active = ui->activeCheckbox->isChecked();
	mTelephone.should_register_startup = ui->registerStartupCheckbox->isChecked();
	mTelephone.transport = ui->transportCombobox->currentIndex() + 1;

	bool shouldEnableInputs = true;
	if (mTelephone.active) {
		shouldEnableInputs = false;
	}

	ui->descriptionEdit->setEnabled(shouldEnableInputs);
	ui->nameEdit->setEnabled(shouldEnableInputs);
	ui->domainEdit->setEnabled(shouldEnableInputs);
	ui->usernameEdit->setEnabled(shouldEnableInputs);
	ui->passwordEdit->setEnabled(shouldEnableInputs);

	mManager->updateKTelephone(oldUsername, &mTelephone);
	mTelephones = mManager->getTelephones();

	if (currentTelephone) {
		currentTelephone->setText(mTelephone.username);
		ui->telephonesList->editItem(currentTelephone);
	}

	if (shouldUpdateInstance && mTelephone.active) {
		mTelephones.value(mTelephone.username)->show();
	} else if (shouldUpdateInstance && !mTelephone.active) {
		mTelephones.value(mTelephone.username)->hide();
	}
}

void KTelephonePreferences::newItem() {
	Telephone_t telephone{
			.id=QString("0"),
			.description=QString("dummy"),
			.name=QString("dummy"),
			.domain=QString("0.0.0.0"),
			.username=QString("dummy@0.0.0.0"),
			.password=QString("dummypass"),
			.active=0, // active

			// prefs
			.should_register_startup=1,
			.should_subscribe_presence=0,
			.should_publish_presence=0,
			.should_use_blf=0,
			.should_disable_ringback_tone=0,
			.custom_ringtone=QString(""),

			.transport=1,
			.subscription_expiry_delay=0,
			.keep_alive_expiry_delay=0,
			.registration_expiry_delay=0,
			.use_stun=0,
			.stun_server=QString("")
	};

	mManager->saveTelephone(&telephone);
	mManager->newKTelephone(telephone);
	ui->telephonesList->addItem(telephone.username);
}

void KTelephonePreferences::removeItem() {
	if (currentTelephone == NULL) {
		return;
	}

	QHash<QString, KTelephone *> mTelephones = mManager->getTelephones();
	if (!mTelephones.contains(currentTelephone->text())) {
		qDebug() << "No item selected";
		return;
	}

	KTelephone *item = mTelephones.value(currentTelephone->text());
	Telephone_t *mTelephone = &item->mTelephone;

	mManager->deleteTelephone(mTelephone);
	mManager->removeKTelephone(mTelephone);
	delete ui->telephonesList->takeItem(ui->telephonesList->row(currentTelephone));

	currentTelephone = NULL;
}
