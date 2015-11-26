#include "comparisondialog.h"

#include "mainwindow.h"
#include <iostream>

ComparisonDialog::ComparisonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComparisonDialog)
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    ui->setupUi(this);

    QObject::connect(this->ui->sta_menu_filter,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SLOT(sta_menuFilterSelected(int)));

    QObject::connect(this->ui->sta_recipe_filter,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SLOT(sta_recipeFilterSelected(int)));

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

void ComparisonDialog::sta_menuFilterSelected(int index)
{
    MainWindow * mainwindow = (MainWindow *) this->parent();
    mainwindow->sta_menuFilterSelected(index);
}

void ComparisonDialog::sta_recipeFilterSelected(int index)
{
    MainWindow * mainwindow = (MainWindow *) this->parent();
    mainwindow->sta_recipeFilterSelected(index);
}
