#ifndef CALL_H
#define CALL_H

#include <QDialog>

namespace Ui {
  class call;
}

class KTelephoneCall : public QDialog
{
  Q_OBJECT

 public:
   explicit KTelephoneCall(QWidget *parent = 0);
   ~KTelephoneCall();

 private:
   Ui::call *ui;

};


#endif // CALL_H
