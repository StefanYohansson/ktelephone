#include "ktelephone.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ktelephone w;
    w.show();

    return app.exec();
}

