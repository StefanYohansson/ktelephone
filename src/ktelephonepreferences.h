#ifndef KTELEPHONEPREFERENCES_H
#define KTELEPHONEPREFERENCES_H

#include <QDialog>

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
   
};

#endif // KTELEPHONEPREFERENCES_H
