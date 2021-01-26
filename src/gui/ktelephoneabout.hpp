#ifndef KTELEPHONE_KTELEPHONEABOUT_HPP
#define KTELEPHONE_KTELEPHONEABOUT_HPP

#include <QDialog>
#include "ui_about.h"

namespace Ui {
	class about;
}

class KTelephoneAbout : public QDialog {
Q_OBJECT

public:
	explicit KTelephoneAbout(QWidget *parent = 0);

	~KTelephoneAbout();

private:
	Ui::about *ui;
};


#endif //KTELEPHONE_KTELEPHONEABOUT_HPP
