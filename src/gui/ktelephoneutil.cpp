#include <QRegularExpression>
#include "ktelephoneutil.hpp"

QString getNumberFromURI(QString uri) {
    QRegularExpression re("sip:(?<number>.+)@.+");
    QRegularExpressionMatch match = re.match(uri);
    QString number = match.captured("number");
    return number;
}
