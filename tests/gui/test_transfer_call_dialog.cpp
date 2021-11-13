#include <QtTest>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>

#include "ktelephonetransfercall.hpp"


class TestTransferCallDialog: public QObject {
Q_OBJECT

private slots:
    void testCallTransferCallFunction() {
        QString value;

        auto onTransferCallClick = [&](QString destinatinoNumber) mutable {
            value = destinatinoNumber;
        };

        KTelphoneTransferCall dialog(onTransferCallClick);
        QLineEdit* destinationNumber = dialog.findChild<QLineEdit*>("destinationNumberInput");
        QPushButton* transferButton = dialog.findChild<QPushButton*>("transferButton");

        QTest::keyClicks(destinationNumber, "1000");
        QTest::mouseClick(transferButton, Qt::LeftButton);

        QCOMPARE("1000", value);
    }

    void testTransferButtonIsDefault() {
        KTelphoneTransferCall dialog(NULL);
        QPushButton* transferButton = dialog.findChild<QPushButton*>("transferButton");

        QCOMPARE(true, transferButton->isDefault());
    }

    void testResetStateOnEscape() {
        KTelphoneTransferCall dialog(NULL);
        QLineEdit* destinationNumber = dialog.findChild<QLineEdit*>("destinationNumberInput");

        QTest::keyClicks(destinationNumber, "1000");
        QCOMPARE("1000", destinationNumber->text());

        QTest::keyEvent(QTest::KeyAction::Press, &dialog, Qt::Key_Escape);

        QCOMPARE("", destinationNumber->text());
    }
};


QTEST_MAIN(TestTransferCallDialog)

#include "test_transfer_call_dialog.moc"