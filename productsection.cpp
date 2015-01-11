#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QtSql>
#include <QStandardItemModel>

#include "pricedialog.h"

void MainWindow::makeProductConnections()
{
    QObject::connect(this->ui->prod_newprice,
                     SIGNAL(clicked()),
                     this,
                     SLOT(showNewPricePopup()));
}

void MainWindow::buildProductTree()
{
    QStandardItemModel * rootModel = new QStandardItemModel(this);

    QSqlQuery queryCat;
    QSqlQuery querySubcat;
    QSqlQuery queryProduct;

    std::stringstream query_str;

    query_str << "SELECT id, name FROM prod_cat" << std::endl;
    queryCat.exec(query_str.str().c_str());

    while (queryCat.next())
    {
        QStandardItem * category = new QStandardItem(queryCat.value("name").toString());
        int catId = queryCat.value("id").toInt();
        category->setData(QVariant::fromValue((int) SK_Category), SK_TypeRole);
        category->setData(QVariant::fromValue(catId), SK_IdRole);
        rootModel->appendRow(category);

        query_str.str(std::string());
        query_str.clear();
        query_str << "SELECT id, name FROM prod_subcat" << std::endl;
        query_str << "WHERE cat = " << catId << std::endl;
        querySubcat.exec(query_str.str().c_str());

        while (querySubcat.next())
        {
            QStandardItem * subcategory = new QStandardItem(querySubcat.value("name").toString());
            int subCatId = querySubcat.value("id").toInt();
            subcategory->setData(QVariant::fromValue((int) SK_Subcategory), SK_TypeRole);
            subcategory->setData(QVariant::fromValue(subCatId), SK_IdRole);
            category->appendRow(subcategory);

            query_str.str(std::string());
            query_str.clear();
            query_str << "SELECT id, name FROM product" << std::endl;
            query_str << "WHERE cat = " << catId << std::endl;
            query_str << "AND subcat = " << subCatId << std::endl;
            queryProduct.exec(query_str.str().c_str());

            while (queryProduct.next())
            {
                QStandardItem * product = new QStandardItem(queryProduct.value("name").toString());
                product->setData(QVariant::fromValue((int) SK_Product), SK_TypeRole);
                product->setData(QVariant::fromValue(queryProduct.value("id").toInt()), SK_IdRole);
                subcategory->appendRow(product);
            }
        }

//        query_str.str(std::string());
//        query_str.clear();
//        query_str << "SELECT id, name FROM product" << std::endl;
//        query_str << "WHERE cat = " << catId << std::endl;
//        query_str << "AND subcat IS NULL" << std::endl;
//        queryProduct.exec(query_str.str().c_str());

//        while (queryProduct.next())
//        {
//            QStandardItem * product = new QStandardItem(queryProduct.value("name").toString());
//            product->setData(QVariant::fromValue((int) SK_Product), SK_TypeRole);
//            product->setData(QVariant::fromValue(queryProduct.value("id").toInt()), SK_IdRole);
//            category->appendRow(product);
//        }
    }

    this->ui->prod_tree->setModel(rootModel);

}

//void MainWindow::buildProductTree()
//{
//    QStandardItemModel * rootModel = new QStandardItemModel(this);

//    QSqlQuery query;
//    std::vector<std::string> queries;
//    std::vector<std::string> names;
//    std::vector<int> types;

//    queries.push_back("SELECT id, name FROM product");
//    names.push_back("Products");
//    types.push_back(SK_Product);

//    queries.push_back("SELECT id, name FROM recipe");
//    names.push_back("Recipes");
//    types.push_back(SK_Recipe);

//    queries.push_back("SELECT id, name FROM menu");
//    names.push_back("Menus");
//    types.push_back(SK_Menu);

//    for (int i = 0; i < queries.size(); i++)
//    {
//        query.prepare(queries.at(i).c_str());

//        query.exec();

//        QStandardItem * wrapper = new QStandardItem(names.at(i).c_str());
//        wrapper->setData(QVariant::fromValue((int) SK_Wrapper), SK_TypeRole);
//        wrapper->setData(QVariant::fromValue(types.at(i)), SK_IdRole);
//        rootModel->appendRow(wrapper);

//        while (query.next())
//        {
//            QStandardItem * item = new QStandardItem(query.value("name").toString());
//            item->setData(QVariant::fromValue(query.value("id").toInt()), SK_IdRole);
//            item->setData(QVariant::fromValue(types.at(i)), SK_TypeRole);
//            wrapper->appendRow(item);
//        }
//    }

//    this->ui->prod_tree->setModel(rootModel);
//    this->ui->prod_tree->expand(this->ui->prod_tree->model()->index(0, 0));
//}


void MainWindow::productSelected(const QModelIndex &current, const QModelIndex &previous)
{
    int type = current.data(SK_TypeRole).toInt();
    int id = current.data(SK_IdRole).toInt();
    QTreeView * tree = this->ui->prod_tree;

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
        query_str << "SELECT name, product.notes, cat, subcat, price FROM product INNER JOIN prod_price ON product.id = prod_price.product" << std::endl;
        query_str << "WHERE product.id = " << id << std::endl;
        query.exec(query_str.str().c_str());
        if (query.next())
        {
            this->ui->prod_name->setText(query.value("name").toString());
            int cat = -1;
            int subcat = -1;
            cat = (query.value("cat").isNull()) ? -1 : query.value("cat").toInt();
            subcat = (query.value("subcat").isNull()) ? -1 : query.value("subcat").toInt();
            fillProductCategoryLists(cat, subcat);
        }
    }
}

void MainWindow::fillProductCategoryLists(int catId, int subCatId)
{
    this->ui->prod_cat->clear();
    this->ui->prod_subcat->clear();

    QSqlQuery query;
    std::stringstream query_str;
    query_str << "SELECT id, name FROM prod_cat" << std::endl;
    query.exec(query_str.str().c_str());
    int selected = 0, index = 1;
    this->ui->prod_cat->addItem("None", QVariant::fromValue(-1));
    while (query.next())
    {
        this->ui->prod_cat->addItem(query.value("name").toString(), QVariant::fromValue(query.value("id").toInt()));
        if (query.value("id").toInt() == catId)
            selected = index;
        index++;
    }
    this->ui->prod_cat->setCurrentIndex(selected);

    if (catId != -1)
    {
        QSqlQuery query;
        std::stringstream query_str;
        query_str << "SELECT id, name FROM prod_subcat" << std::endl;
        query_str << "WHERE cat = " << catId << std::endl;
        query.exec(query_str.str().c_str());
        int selected = 0, index = 1;
        this->ui->prod_subcat->addItem("None", QVariant::fromValue(-1));
        while (query.next())
        {
            this->ui->prod_subcat->addItem(query.value("name").toString(), QVariant::fromValue(query.value("id").toInt()));
            if (query.value("id").toInt() == subCatId)
                selected = index;
            index++;
        }
        this->ui->prod_subcat->setCurrentIndex(selected);
    }
    else
    {
        this->ui->prod_subcat->addItem("None", QVariant::fromValue(-1));
    }
}

void MainWindow::showNewPricePopup()
{
    QDialog* popUp= new PriceDialog();
    popUp->setModal(true);
    popUp->show();
}
