#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QApplication>
#include <QTextStream>
#include <QInputDialog>
#include <QIODevice>
#include <QString>
#include <QFile>
#include <QMessageBox>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void change_it(int it);
    void apply();

private:
    Ui::Widget *ui;
    int nb_it=0;
};

#endif // WIDGET_H
