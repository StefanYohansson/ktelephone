#ifndef KTELEPHONEGUIDE_H
#define KTELEPHONEGUIDE_H

#include <QPushButton>
#include <QLineEdit>
#include <QDialog>

class KTelephoneManager;

namespace Ui {
    class guide;
}

class KTelephoneGuide : public QDialog {
Q_OBJECT

public:
    explicit KTelephoneGuide(QWidget *parent = 0);

    ~KTelephoneGuide();

    void setManager(KTelephoneManager *manager);

private:
    Ui::guide *ui;

protected:
    KTelephoneManager *mManager;

public slots:

    void accept();
	void usernameChanged(const QString &text);

};

#endif // KTELEPHONEGUIDE_H
