#include "setcomunicationwindow.h"
#include "ui_setcomunicationwindow.h"

setcomunicationwindow::setcomunicationwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setcomunicationwindow)
{
    ui->setupUi(this);
}

setcomunicationwindow::~setcomunicationwindow()
{
    delete ui;
}
