#ifndef KTELEPHONETRANSFERCALL_H
#define KTELEPHONETRANSFERCALL_H

#include <QDialog>
#include <QString>
#include "ui_transfercall.h"

namespace Ui {
	class transfercall;
}

class KTelphoneTransferCall : public QDialog {
Q_OBJECT

public:
	explicit KTelphoneTransferCall(std::function<void(QString)> onTransferClick);

	~KTelphoneTransferCall();

private:
	Ui::transfercall *ui;
	std::function<void(QString)> onTransferClick;


protected:
	void closeEvent(QCloseEvent *);

private slots:

	void actionTransfer();

	void actionCancel();

	void resetState();
};

#endif // KTELEPHONETRANSFERCALL_H
