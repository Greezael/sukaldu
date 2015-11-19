#include "comparisondialog.h"

#include "mainwindow.h"
#include <iostream>

ComparisonDialog::ComparisonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComparisonDialog)
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    ui->setupUi(this);
}

ComparisonDialog::~ComparisonDialog()
{
    MainWindow * mainwindow = (MainWindow *) this->parent();
    mainwindow->compDialog = nullptr;

    delete ui;
}
