#include <QApplication>

#include "ktelephone.h"
#include "ktelephonemanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KTelephoneManager m;
    return app.exec();
}

