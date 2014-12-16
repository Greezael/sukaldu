#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSql>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
//    query.prepare("SELECT * FROM product");
    query.prepare("SELECT name, product.notes, price FROM product INNER JOIN prod_price ON product.id = prod_price.product");

    query.exec();

    model->setQuery(query);
//    model->setHeaderData(0, Qt::Horizontal, "Col1");

    this->ui->tableView->setModel(model);

}

MainWindow::~MainWindow()
{
    delete ui;
}
