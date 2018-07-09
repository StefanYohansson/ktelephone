#ifndef KTELEPHONE_H
#define KTELEPHONE_H

#include <QMainWindow>

namespace Ui {
class ktelephone;
}

class ktelephone : public QMainWindow
{
    Q_OBJECT

public:
    explicit ktelephone(QWidget *parent = 0);
    ~ktelephone();

private:
    Ui::ktelephone *ui;
};

#endif // KTELEPHONE_H
