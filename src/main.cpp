#include <QApplication>

#include "ktelephone.hpp"
#include "ktelephonemanager.hpp"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	const auto appIcon = QIcon("data/ktelephone.png");
	app.setWindowIcon(appIcon);
	KTelephoneManager m;
	return app.exec();
}
