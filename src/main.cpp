#include <QApplication>

#include "ktelephone.hpp"
#include "ktelephonemanager.hpp"

QString readStyleSheet(QString path) {
    QFile file(path);
    file.open(QFile::ReadOnly);
    return QString(file.readAll());
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QIcon appIcon("data/ktelephone.png");
    app.setWindowIcon(appIcon);
    app.setStyleSheet(readStyleSheet(":/qss/style.qss"));

    KTelephoneManager m;
    return app.exec();
}
