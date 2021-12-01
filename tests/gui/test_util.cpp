#include <QtTest>
#include "ktelephoneutil.hpp"

class TestUtil: public QObject {
Q_OBJECT

private slots:
    void testGetNumberFromURI() {
        QCOMPARE(getNumberFromURI(QString("sip:5000@127.0.0.1")), QString("5000"));
        QCOMPARE(getNumberFromURI(QString("<sip:5000@127.0.0.1;ob>")), QString("5000"));
        QCOMPARE(getNumberFromURI(QString("\"5000\" sip:5000@127.0.0.1")), QString("5000"));
        QCOMPARE(getNumberFromURI(QString("\"5000\" <sip:5000@127.0.0.1;ob>")), QString("5000"));
    }
};

QTEST_MAIN(TestUtil)

#include "test_util.moc"