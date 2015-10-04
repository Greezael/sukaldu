
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QtSql>
#include <QStandardItemModel>

void MainWindow::sta_loadTables()
{
    sta_loadTableMenu();
    sta_loadTableRecipe();
}

void MainWindow::sta_loadTableRecipe()
{
    QStandardItemModel * rootModel;
    if (this->ui->sta_recipe->model() != NULL)
    {
        rootModel = (QStandardItemModel *) this->ui->sta_recipe->model();
        rootModel->clear();
    }
    else
    {
        rootModel = new QStandardItemModel(this);
    }

    QSqlQuery query;

    query.prepare("SELECT P.id, P.name, RP.price, C.name 'cat', SC.name 'subcat' "
                  "FROM recipe P "
                  "JOIN C_recipe_price RP "
                  "ON P.id = RP.id "
                  "LEFT JOIN recipe_cat C "
                  "ON C.id = P.cat "
                  "LEFT JOIN recipe_subcat SC "
                  "ON SC.id = P.subcat "
                  );
    query.exec();
//    rootModel->setColumnCount(2);
    while (query.next())
    {
        QList<QStandardItem*> row;
        QStandardItem* name = new QStandardItem(query.value("name").toString());
        QStandardItem* price = new QStandardItem(currencyFormatter(query.value("price")));
        QStandardItem* cat = new QStandardItem(query.value("cat").toString());
        QStandardItem* subcat = new QStandardItem(query.value("subcat").toString());
        row << name << price << cat << subcat;
        name->setData(query.value("id"), SK_IdRole);

        rootModel->appendRow(row);
    }

    rootModel->setHorizontalHeaderLabels(QList<QString>() << tr("Name") << tr("Price") << tr("Category") << tr("Subcategory"));
    this->ui->sta_recipe->setModel(rootModel);
}

void MainWindow::sta_loadTableMenu()
{
    QStandardItemModel * rootModel;
    if (this->ui->sta_menu->model() != NULL)
    {
        rootModel = (QStandardItemModel *) this->ui->sta_menu->model();
        rootModel->clear();
    }
    else
    {
        rootModel = new QStandardItemModel(this);
    }

    QSqlQuery query;

    query.prepare("SELECT M.id, M.name, MP.min_price, MP.avg_price, MP.max_price, C.name 'cat', SC.name 'subcat' "
                  "FROM menu M "
                  "JOIN C_menu_price MP "
                  "ON M.id = MP.id "
                  "LEFT JOIN menu_cat C "
                  "ON C.id = M.cat "
                  "LEFT JOIN menu_subcat SC "
                  "ON SC.id = M.subcat "
                  );
    query.exec();
    while (query.next())
    {
        QList<QStandardItem*> row;
        QStandardItem* name = new QStandardItem(query.value("name").toString());
        QStandardItem* min_price = new QStandardItem(currencyFormatter(query.value("min_price")));
        QStandardItem* avg_price = new QStandardItem(currencyFormatter(query.value("avg_price")));
        QStandardItem* max_price = new QStandardItem(currencyFormatter(query.value("max_price")));
        QStandardItem* cat = new QStandardItem(query.value("cat").toString());
        QStandardItem* subcat = new QStandardItem(query.value("subcat").toString());
        row << name << avg_price << max_price << min_price << cat << subcat;
        name->setData(query.value("id"), SK_IdRole);

        rootModel->appendRow(row);
    }

    rootModel->setHorizontalHeaderLabels(QList<QString>() << tr("Name") << tr("Average") << tr("Most expensive") << tr("Cheapest") << tr("Category") << tr("Subcategory"));
    this->ui->sta_menu->setModel(rootModel);
}
