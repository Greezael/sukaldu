#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QtSql>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    currentProduct = -1;
    currentRecipe = -1;
    currentMenu = -1;

    buildProductTree();
    buildRecipeTree();
    buildMenuTree();

    QObject::connect(this->ui->prod_tree->selectionModel(),
                     SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                     this,
                     SLOT(prodTreeItemSelected(const QModelIndex &, const QModelIndex &)));
    makeProductConnections();

}

MainWindow::~MainWindow()
{
    delete ui;
}






