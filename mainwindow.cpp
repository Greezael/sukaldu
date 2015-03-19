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

    makeConnections();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeConnections()
{
    // Tabs
    QObject::connect(this->ui->tabWidget,
                     SIGNAL(currentChanged(int)),
                     this,
                     SLOT(tabChanged(int)));

    // Trees
    QObject::connect(this->ui->prod_tree->selectionModel(),
                     SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                     this,
                     SLOT(treeItemSelected(const QModelIndex &, const QModelIndex &)));
    QObject::connect(this->ui->rec_tree->selectionModel(),
                     SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                     this,
                     SLOT(treeItemSelected(const QModelIndex &, const QModelIndex &)));
    QObject::connect(this->ui->menu_tree->selectionModel(),
                     SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                     this,
                     SLOT(treeItemSelected(const QModelIndex &, const QModelIndex &)));

    // Categories
    QObject::connect(this->ui->prod_cat,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SLOT(prodCatSelected(int)));
    QObject::connect(this->ui->rec_cat,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SLOT(recCatSelected(int)));

    // Reset and Save Buttons
    QObject::connect(this->ui->prod_curprod_buttons,
                     SIGNAL(clicked(QAbstractButton*)),
                     this,
                     SLOT(generalProdButtonClicked(QAbstractButton*)));
    QObject::connect(this->ui->rec_currec_buttons,
                     SIGNAL(clicked(QAbstractButton*)),
                     this,
                     SLOT(generalRecButtonClicked(QAbstractButton*)));

    // Delete item Buttons
    QObject::connect(this->ui->prod_delprod,
                     SIGNAL(clicked()),
                     this,
                     SLOT(deleteProduct()));
    QObject::connect(this->ui->rec_delrec,
                     SIGNAL(clicked()),
                     this,
                     SLOT(deleteRecipe()));

    // Insert item Buttons
    QObject::connect(this->ui->prod_newprod,
                     SIGNAL(clicked()),
                     this,
                     SLOT(insertNewProduct()));
    QObject::connect(this->ui->rec_newrec,
                     SIGNAL(clicked()),
                     this,
                     SLOT(insertNewRecipe()));

    // Product
    QObject::connect(this->ui->prod_newprice,
                     SIGNAL(clicked()),
                     this,
                     SLOT(showNewPricePopup()));
    QObject::connect(this->ui->prod_editprice,
                     SIGNAL(clicked()),
                     this,
                     SLOT(showEditPricePopup()));
    QObject::connect(this->ui->prod_delprice,
                     SIGNAL(clicked()),
                     this,
                     SLOT(deletePrice()));
    QObject::connect(this->ui->prod_setcurrentprice,
                     SIGNAL(clicked()),
                     this,
                     SLOT(setCurrentPrice()));

    // Recipe
    QObject::connect(this->ui->rec_addprod,
                     SIGNAL(clicked()),
                     this,
                     SLOT(showAddProductPopup()));
    QObject::connect(this->ui->rec_editprod,
                     SIGNAL(clicked()),
                     this,
                     SLOT(showEditProductPopup()));
    QObject::connect(this->ui->rec_remprod,
                     SIGNAL(clicked()),
                     this,
                     SLOT(removeProduct()));

}




