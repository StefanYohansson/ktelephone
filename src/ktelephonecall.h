#ifndef CALL_H
#define CALL_H

#include <QDialog>

class MyCall;

#include "ktelephone.h"

namespace Ui {
  class call;
}

class KTelephoneCall : public QDialog
{
  Q_OBJECT

 public:
   explicit KTelephoneCall(KTelephone *parent = 0);
   ~KTelephoneCall();
   void setInstance(MyCall* telephoneCall);
   void callDestroy();

 private:
   Ui::call *ui;
   MyCall *mCall = NULL;

 public slots:
  void actionHangup();
  void actionAnswer();

};


#endif // CALL_H
