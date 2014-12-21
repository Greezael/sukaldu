#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QtSql>
#include <QStandardItemModel>

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


//    this->ui->treeView->setModel(model);
    buildTree();

    QObject::connect(this->ui->treeView->selectionModel(),
                     SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                     this,
                     SLOT(treeItemSelected(const QModelIndex &, const QModelIndex &)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buildTree()
{
    QStandardItemModel * rootModel = new QStandardItemModel(this);

    QSqlQuery query;
    std::vector<std::string> queries;
    std::vector<std::string> names;
    std::vector<int> types;

    queries.push_back("SELECT id, name FROM product");
    names.push_back("Products");
    types.push_back(SK_Product);

    queries.push_back("SELECT id, name FROM recipe");
    names.push_back("Recipes");
    types.push_back(SK_Recipe);

    queries.push_back("SELECT id, name FROM menu");
    names.push_back("Menus");
    types.push_back(SK_Menu);

    for (int i = 0; i < queries.size(); i++)
    {
        query.prepare(queries.at(i).c_str());

        query.exec();

        QStandardItem * wrapper = new QStandardItem(names.at(i).c_str());
        wrapper->setData(QVariant::fromValue((int) SK_Wrapper), SK_TypeRole);
        rootModel->appendRow(wrapper);

        while (query.next())
        {
            QStandardItem * item = new QStandardItem(query.value("name").toString());
            item->setData(QVariant::fromValue(query.value("id").toInt()), SK_IdRole);
            item->setData(QVariant::fromValue(types.at(i)), SK_TypeRole);
            wrapper->appendRow(item);
        }
    }

    this->ui->treeView->setModel(rootModel);
}

void MainWindow::treeItemSelected(const QModelIndex &current, const QModelIndex &previous) {
//    std::cout << "Row: " << current.row() << ", Col: " << current.column() << std::endl;
    int type = current.data(SK_TypeRole).toInt();
    std::cout << "Type: " << type << std::endl;
    int id = current.data(SK_IdRole).toInt();
    std::cout << "Id: " << id << std::endl;


}
