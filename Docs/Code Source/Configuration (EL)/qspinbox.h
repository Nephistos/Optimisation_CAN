#ifndef QSPINBOX_H
#define QSPINBOX_H

namespace Ui {
class QSpinBox;
}

class QSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    explicit QSpinBox(QSpinBox *parent = 0);
    ~QSpinBox();

private:
    Ui::QSpinBox *ui;
};

#endif // QSPINBOX_H
