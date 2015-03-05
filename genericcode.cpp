#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QtSql>
#include <QStandardItemModel>

void MainWindow::buildTree(SK_Section section)
{
    QTreeView* tree;
    QString prefix;
    SK_ObjectType role;
    switch (section)
    {
    case SK_S_PROD:
        tree = this->ui->prod_tree;
        prefix = "prod";
        role = SK_Product;
        break;
    case SK_S_REC:
        tree = this->ui->rec_tree;
        prefix = "recipe";
        role = SK_Recipe;
        break;
    case SK_S_MENU:
        tree = this->ui->menu_tree;
        prefix = "menu";
        role = SK_Menu;
        break;
    }

    QStandardItemModel * rootModel;
    if (tree->model() != NULL)
    {
        rootModel = (QStandardItemModel *) tree->model();
        rootModel->clear();
    }
    else
    {
        rootModel = new QStandardItemModel(this);
    }

    tree->blockSignals(true);


    QSqlQuery queryCat;
    QSqlQuery querySubcat;
    QSqlQuery queryItem;

    queryCat.prepare("SELECT id, name FROM " + prefix + "_cat");
    queryCat.exec();

    while (queryCat.next())
    {
        QStandardItem * category = new QStandardItem(queryCat.value("name").toString());
        QVariant catId = queryCat.value("id");
        category->setData(QVariant::fromValue((int) SK_Category), SK_TypeRole);
        category->setData(catId, SK_IdRole);
        rootModel->appendRow(category);

        querySubcat.prepare("SELECT id, name FROM " + prefix + "_subcat "
                            "WHERE cat = :cat");
        querySubcat.bindValue(":cat", catId);
        querySubcat.exec();

        while (querySubcat.next())
        {
            QStandardItem * subcategory = new QStandardItem(querySubcat.value("name").toString());
            QVariant subCatId = querySubcat.value("id");
            subcategory->setData(QVariant::fromValue((int) SK_Subcategory), SK_TypeRole);
            subcategory->setData(subCatId, SK_IdRole);
            category->appendRow(subcategory);

            queryItem.prepare("SELECT id, name FROM " + prefix + " "
                                 "WHERE cat = :catId AND subcat = :subCatId");
            queryItem.bindValue(":catId", catId);
            queryItem.bindValue(":subCatId", subCatId);
            queryItem.exec();

            while (queryItem.next())
            {
                QStandardItem * item = new QStandardItem(queryItem.value("name").toString());
                item->setData(QVariant::fromValue((int) role), SK_TypeRole);
                item->setData(QVariant::fromValue(queryItem.value("id").toInt()), SK_IdRole);
                subcategory->appendRow(item);
                if (queryItem.value("id").toInt() == currentProduct)
                {
                    tree->setExpanded(rootModel->indexFromItem(category), true);
                    tree->setExpanded(rootModel->indexFromItem(subcategory), true);
                    tree->selectionModel()->setCurrentIndex(rootModel->indexFromItem(item),
                                                                           QItemSelectionModel::SelectCurrent);
                }
            }
        }

        queryItem.prepare("SELECT id, name FROM " + prefix + " "
                             "WHERE cat = :catId AND subcat IS NULL");
        queryItem.bindValue(":catId", catId);
        queryItem.exec();

        while (queryItem.next())
        {
            QStandardItem * item = new QStandardItem(queryItem.value("name").toString());
            item->setData(QVariant::fromValue((int) role), SK_TypeRole);
            item->setData(QVariant::fromValue(queryItem.value("id").toInt()), SK_IdRole);
            category->appendRow(item);
            if (queryItem.value("id").toInt() == currentProduct)
            {
                tree->setExpanded(rootModel->indexFromItem(category), true);
                tree->selectionModel()->setCurrentIndex(rootModel->indexFromItem(item),
                                                                       QItemSelectionModel::SelectCurrent);
            }
        }
    }

    queryItem.prepare("SELECT id, name FROM " + prefix + " "
                         "WHERE cat IS NULL AND subcat IS NULL");
    queryItem.exec();

    while (queryItem.next())
    {
        QStandardItem * item = new QStandardItem(queryItem.value("name").toString());
        item->setData(QVariant::fromValue((int) role), SK_TypeRole);
        item->setData(QVariant::fromValue(queryItem.value("id").toInt()), SK_IdRole);
        rootModel->appendRow(item);
        if (queryItem.value("id").toInt() == currentProduct)
        {
            tree->selectionModel()->setCurrentIndex(rootModel->indexFromItem(item),
                                                                   QItemSelectionModel::SelectCurrent);
        }
    }

    tree->setModel(rootModel);
    tree->blockSignals(false);

}
