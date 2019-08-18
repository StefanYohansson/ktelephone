#include "ktelephone.h"
#include "ktelephonepreferences.h"
#include "ui_settings.h"

#include <QDebug>

KTelephonePreferences::KTelephonePreferences(QWidget *parent) :
    QMainWindow(),
    ui(new Ui::settings)
{
  ui->setupUi(this);

  connect(ui->telephonesList,
          SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this,
          SLOT(itemChanged(QListWidgetItem*, QListWidgetItem*)));
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

KTelephonePreferences::~KTelephonePreferences()
{
  delete ui;
}

void KTelephonePreferences::setManager(KTelephoneManager *manager)
{
  if (!mManager) {
    return;
  }

  mManager = manager;

  mTelephones = mManager->getTelephones();

  foreach( QString item, mTelephones.keys() ) {
    ui->telephonesList->addItem(mTelephones[item]->mTelephone.username);
  }
}

void KTelephonePreferences::itemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
  if (!current || !mTelephones.contains(current->text())) {
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

  currentTelephone = current;
}

void KTelephonePreferences::saveChanges()
{
  const bool shouldUpdateInstance = mTelephone.active != ui->activeCheckbox->checkState();
  const QString oldUsername = mTelephone.username;
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

  mManager->updateKTelephone(oldUsername, mTelephone);
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

void KTelephonePreferences::newItem()
{
  Telephone_t telephone {
    QString("0"),
    QString("dummy"),
    QString("dummy"),
    QString("0.0.0.0"),
    QString("dummy@0.0.0.0"),
    QString("dummypass"),
    0 // active
  };

  mManager->saveTelephone(&telephone);
  mManager->newKTelephone(telephone);
  ui->telephonesList->addItem(telephone.username);
  mTelephones = mManager->getTelephones();
}

void KTelephonePreferences::removeItem()
{
  if (!currentTelephone || !mTelephones.contains(currentTelephone->text())) {
    qDebug() << "No item selected";
    return;
  }

  const KTelephone *item = mTelephones.value(currentTelephone->text());
  mTelephone = item->mTelephone;

  mManager->deleteTelephone(mTelephone);
  mManager->removeKTelephone(mTelephone);
  delete ui->telephonesList->takeItem(ui->telephonesList->row(currentTelephone));

  currentTelephone = NULL;
}
