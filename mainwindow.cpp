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


    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
//    query.prepare("SELECT name, product.notes, price FROM product INNER JOIN prod_price ON product.id = prod_price.product");
    query.prepare("SELECT 'recipe', * FROM C_recipe_price \
                  UNION SELECT 'menu', * FROM C_menu_price \
                  UNION SELECT 'product', * FROM C_prod_price");

    query.exec();

    model->setQuery(query);
//    model->setHeaderData(0, Qt::Horizontal, "Col1");

    this->ui->tableView->setModel(model);

    buildProductTree();

    QObject::connect(this->ui->prod_tree->selectionModel(),
                     SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                     this,
                     SLOT(productSelected(const QModelIndex &, const QModelIndex &)));
    makeProductConnections();

}

MainWindow::~MainWindow()
{
    delete ui;
}






