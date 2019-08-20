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
   explicit KTelephoneCall(KTelephone *parent = 0, QString direction = "inbound");
   ~KTelephoneCall();
   void setInstance(MyCall* telephoneCall);
   void callDestroy();

 private:
   Ui::call *ui;
   MyCall *mCall = NULL;
   QString callDirection;
   bool hold = false;
   bool mute = false;

 public slots:
  void actionHangup();
  void actionAnswer();
  void actionMute();
  void actionHold();
  void actionTransfer();

};


#endif // CALL_H
