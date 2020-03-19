#ifndef KTELEPHONE_H
#define KTELEPHONE_H

#include <QMainWindow>
#include <QLabel>
#include <QShortcut>

#include "ktelephonemanager.h"
#include "ktelephonepreferences.h"
#include "mycall.h"

class KTelephoneManager;

namespace Ui {
  class ktelephone;
}

class KTelephone : public QMainWindow
{
    Q_OBJECT

public:
    explicit KTelephone(QWidget *parent = 0);
    ~KTelephone();
    void setManager(KTelephoneManager *manager);
    void setTelephone(Telephone_t telephone);
    void statusMessage(QString);
    void changeRegistrationStatus(bool status);
    KTelephoneManager* getManager();
    Telephone_t mTelephone;
    QLabel* statusLabel;

private:
    Ui::ktelephone *ui;
    KTelephonePreferences *preferences;

 protected:
    KTelephoneManager *mManager = NULL;

 public slots:
    void actionPreferences();
    void actionAbout();
    void changeStatus(int index);
    void actionInboundCall(MyCall* call);
    void actionOutboundCall();
};

#endif // KTELEPHONE_H
