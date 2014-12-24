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
        wrapper->setData(QVariant::fromValue(types.at(i)), SK_IdRole);
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
    this->ui->treeView->expand(this->ui->treeView->model()->index(0, 0));
}

void MainWindow::treeItemSelected(const QModelIndex &current, const QModelIndex &previous)
{
    int type = current.data(SK_TypeRole).toInt();
    int id = current.data(SK_IdRole).toInt();
    QStackedWidget * editPane = this->ui->edit_pane;
    QTreeView * tree = this->ui->treeView;

    if (type == SK_Wrapper)
    {
//        QModelIndex parentIndex = current.parent();
//        for (qint32 row = 0; row < tree->model()->rowCount(parentIndex); row++)
//        {
//            tree->collapse(tree->model()->index(row, 0, parentIndex));
//        }
//        if (this->ui->treeView->isExpanded(current))
//        {
//            this->ui->treeView->collapse(current);
//        }
//        else
//        {
//            this->ui->treeView->expand(current);
//        }
    }
    else if (type == SK_Product)
    {
        QSqlQuery query;
        std::stringstream query_str;
        query_str << "SELECT name, product.notes, price FROM product INNER JOIN prod_price ON product.id = prod_price.product" << std::endl;
        query_str << "WHERE product.id = " << id << std::endl;
        query.exec(query_str.str().c_str());
        if (query.next())
        {
            this->ui->p_name->setText(query.value("name").toString());
        }
        editPane->setCurrentIndex(editPane->indexOf(this->ui->product_edit));
    }
    else if (type == SK_Recipe)
    {
        editPane->setCurrentIndex(editPane->indexOf(this->ui->recipe_edit));
    }
    else if (type == SK_Menu)
    {
        editPane->setCurrentIndex(editPane->indexOf(this->ui->menu_edit));
    }
}
