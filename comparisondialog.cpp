#include "comparisondialog.h"

#include "mainwindow.h"
#include <iostream>

ComparisonDialog::ComparisonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComparisonDialog)
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    ui->setupUi(this);

    MainWindow * mainwindow = (MainWindow *) this->parent();
    if (!mainwindow->compDialogGeometry.isEmpty())
    {
        this->setGeometry(mainwindow->compDialogGeometry);
    }
}

ComparisonDialog::~ComparisonDialog()
{
    MainWindow * mainwindow = (MainWindow *) this->parent();
    mainwindow->compDialog = nullptr;
    mainwindow->compDialogGeometry = this->geometry();

    delete ui;
}
