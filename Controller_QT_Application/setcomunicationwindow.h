#ifndef SETCOMUNICATIONWINDOW_H
#define SETCOMUNICATIONWINDOW_H

#include <QDialog>

namespace Ui {
class setcomunicationwindow;
}

class setcomunicationwindow : public QDialog
{
    Q_OBJECT

public:
    explicit setcomunicationwindow(QWidget *parent = nullptr);
    ~setcomunicationwindow();

private:
    Ui::setcomunicationwindow *ui;
};

#endif // SETCOMUNICATIONWINDOW_H
