#ifndef KTELEPHONEPREFERENCES_H
#define KTELEPHONEPREFERENCES_H

#include <QDialog>
#include <QListWidgetItem>

#include "ktelephone.h"

namespace Ui {
  class settings;
}

class KTelephonePreferences : public QDialog
{
  Q_OBJECT

 public:
   explicit KTelephonePreferences(KTelephone *parent = 0);
   ~KTelephonePreferences();

 private:
   Ui::settings *ui;

 protected:
   QHash<QString, KTelephone*> mTelephones;
   Telephone_t mTelephone;
   KTelephone *mParent;
   QListWidgetItem *currentTelephone;

 public slots:
   void itemChanged(QListWidgetItem *current, QListWidgetItem *previous);
   void saveChanges();
   void newItem();
   void removeItem();

};

#endif // KTELEPHONEPREFERENCES_H
