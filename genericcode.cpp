#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QtSql>
#include <QStandardItemModel>

void MainWindow::buildTree(SK_Section section)
{
    QTreeView* tree;
    QString prefix, itemTable;
    SK_ObjectType role;
    switch (section)
    {
    case SK_S_PROD:
        tree = this->ui->prod_tree;
        prefix = "prod";
        itemTable = "product";
        role = SK_Product;
        break;
    case SK_S_REC:
        tree = this->ui->rec_tree;
        prefix = "recipe";
        itemTable = "recipe";
        role = SK_Recipe;
        break;
    case SK_S_MENU:
        tree = this->ui->menu_tree;
        prefix = "menu";
        itemTable = "menu";
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

            queryItem.prepare("SELECT id, name FROM " + itemTable + " "
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

        queryItem.prepare("SELECT id, name FROM " + itemTable + " "
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

    queryItem.prepare("SELECT id, name FROM " + itemTable + " "
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

void MainWindow::treeItemSelected(const QModelIndex &current, const QModelIndex &previous)
{
    int type = current.data(SK_TypeRole).toInt();
    int id = current.data(SK_IdRole).toInt();

    if (type == SK_Product)
    {
        productSelected(id);
    }
    else if (type == SK_Recipe)
    {
        recipeSelected(id);
    }
    else if (type == SK_Menu)
    {
//        menuSelected(id);
    }
}

void MainWindow::fillCategoryLists(int catId, int subCatId, SK_Section section)
{
    QComboBox *cat, *subcat;
    QString prefix;
    switch (section)
    {
    case SK_S_PROD:
        cat = this->ui->prod_cat;
        subcat = this->ui->prod_subcat;
        prefix = "prod";
        break;
    case SK_S_REC:
        cat = this->ui->rec_cat;
        subcat = this->ui->rec_subcat;
        prefix = "recipe";
        break;
    case SK_S_MENU:
        cat = this->ui->menu_cat;
        subcat = this->ui->menu_subcat;
        prefix = "menu";
        break;
    }

    this->currentCatId = catId;
    this->currentSubCatId = subCatId;

    cat->blockSignals(true);
    cat->clear();

    QSqlQuery query;
    query.prepare("SELECT id, name FROM " + prefix + "_cat");
    query.exec();
    int selected = 0, index = 1;
    cat->addItem("None", QVariant::fromValue(-1));
    while (query.next())
    {
        cat->addItem(query.value("name").toString(), QVariant::fromValue(query.value("id").toInt()));
        if (query.value("id").toInt() == catId)
            selected = index;
        index++;
    }
    cat->setCurrentIndex(selected);
    cat->blockSignals(false);
    catSelected(selected, section);
}

void MainWindow::catSelected(int index, SK_Section section)
{
    QComboBox *cat, *subcat;
    QString prefix;
    switch (section)
    {
    case SK_S_PROD:
        cat = this->ui->prod_cat;
        subcat = this->ui->prod_subcat;
        prefix = "prod";
        break;
    case SK_S_REC:
        cat = this->ui->rec_cat;
        subcat = this->ui->rec_subcat;
        prefix = "recipe";
        break;
    case SK_S_MENU:
        cat = this->ui->menu_cat;
        subcat = this->ui->menu_subcat;
        prefix = "menu";
        break;
    }

    subcat->clear();
    int catId = cat->itemData(index).toInt();
    if (catId != this->currentCatId)
    {
        this->currentCatId = catId;
        this->currentSubCatId = -1;
    }
    int subCatId = this->currentSubCatId;

    if (catId != -1)
    {
        QSqlQuery query;
        query.prepare("SELECT id, name FROM " + prefix + "_subcat WHERE cat = :catId");
        query.bindValue(":catId", QVariant::fromValue(catId));
        query.exec();
        int selected = 0, index = 1;
        subcat->addItem("None", QVariant::fromValue(-1));
        while (query.next())
        {
            subcat->addItem(query.value("name").toString(), QVariant::fromValue(query.value("id").toInt()));
            if (query.value("id").toInt() == subCatId)
                selected = index;
            index++;
        }
        subcat->setCurrentIndex(selected);
    }
    else
    {
        subcat->addItem("None", QVariant::fromValue(-1));
    }
}
