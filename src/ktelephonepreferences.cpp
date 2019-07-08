#include "ktelephone.h"
#include "ktelephonepreferences.h"
#include "ui_settings.h"

#include <QDebug>

KTelephonePreferences::KTelephonePreferences(KTelephone *parent) :
    QDialog(),
    ui(new Ui::settings)
{
  ui->setupUi(this);

  connect(ui->telephonesList,
          SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this,
          SLOT(itemChanged(QListWidgetItem*, QListWidgetItem*)));
  connect(ui->saveButton,
          SIGNAL(released()), this,
          SLOT(saveChanges()));

  mParent = parent;

  if (!mParent->getManager()) {
    return;
  }

  mTelephones = mParent->getManager()->getTelephones();

  foreach( QString item, mTelephones.keys() ) {
    ui->telephonesList->addItem(mTelephones[item]->mTelephone.domain);
  }
}

KTelephonePreferences::~KTelephonePreferences()
{
  delete ui;
}

void KTelephonePreferences::itemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
  if (!mTelephones.contains(current->text())) {
    return;
  }

  const KTelephone *item = mTelephones.value(current->text());
  mTelephone = item->mTelephone;

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
}

void KTelephonePreferences::saveChanges()
{
  const bool shouldUpdateInstance = mTelephone.active != ui->activeCheckbox->checkState();
  const QString oldDomain = mTelephone.domain;
  mTelephone.description = ui->descriptionEdit->text();
  mTelephone.name = ui->nameEdit->text();
  mTelephone.username = ui->usernameEdit->text();
  mTelephone.password = ui->passwordEdit->text();
  mTelephone.domain = ui->domainEdit->text();
  mTelephone.active = ui->activeCheckbox->isChecked();

  bool shouldEnableInputs = true;
  if (mTelephone.active) {
    shouldEnableInputs = false;
  }

  ui->descriptionEdit->setEnabled(shouldEnableInputs);
  ui->nameEdit->setEnabled(shouldEnableInputs);
  ui->domainEdit->setEnabled(shouldEnableInputs);
  ui->usernameEdit->setEnabled(shouldEnableInputs);
  ui->passwordEdit->setEnabled(shouldEnableInputs);

  mParent->getManager()->updateKTelephone(oldDomain, mTelephone);
  mTelephones = mParent->getManager()->getTelephones();

  if (shouldUpdateInstance && mTelephone.active) {
    mTelephones.value(mTelephone.domain)->show();
  } else if (shouldUpdateInstance && !mTelephone.active) {
    mTelephones.value(mTelephone.domain)->hide();
  }
}
