
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
    sta_loadMenuFilter();
    sta_loadRecipeFilter();
}

void MainWindow::sta_loadTableRecipe()
{
    if (compDialog == nullptr) return;
    QStandardItemModel * rootModel;
    if (compDialog->ui->sta_recipe->model() != NULL)
    {
        rootModel = (QStandardItemModel *) compDialog->ui->sta_recipe->model();
        rootModel->clear();
    }
    else
    {
        rootModel = new QStandardItemModel(this);
    }

    QSqlQuery query;

    QString filterString;
    if (!recipeFilterId.isNull()
        && recipeFilterId.toInt() >= 0)
    {
        filterString = "LEFT JOIN recipe_product RPD "
                "ON P.id = RPD.recipe "
                "WHERE RPD.product = :filterId";
    }

    query.prepare("SELECT P.id, P.name, RP.price, C.name 'cat', SC.name 'subcat' "
                  "FROM recipe P "
                  "JOIN C_recipe_price RP "
                  "ON P.id = RP.id "
                  "LEFT JOIN recipe_cat C "
                  "ON C.id = P.cat "
                  "LEFT JOIN recipe_subcat SC "
                  "ON SC.id = P.subcat " +
                  filterString
                  );

    if (!filterString.isEmpty())
    {
        query.bindValue(":filterId", recipeFilterId.toInt());
    }
    query.exec();

    if (query.lastError().type() != QSqlError::NoError)
    {
        std::cerr << "ERROR: " << query.lastError().text().toUtf8().constData() << std::endl;
        std::cerr << "IN QUERY: " << std::endl;
        std::cerr << QString(query.lastQuery()).toStdString() << std::endl;
        std::cerr << "--------" << std::endl;
    }

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
    compDialog->ui->sta_recipe->setModel(rootModel);
}

void MainWindow::sta_loadTableMenu()
{
    if (compDialog == nullptr) return;

    QStandardItemModel * rootModel;
    if (compDialog->ui->sta_menu->model() != NULL)
    {
        rootModel = (QStandardItemModel *) compDialog->ui->sta_menu->model();
        rootModel->clear();
    }
    else
    {
        rootModel = new QStandardItemModel(this);
    }

    QSqlQuery query;

    QString filterString;
    if (!menuFilterId.isNull()
        && menuFilterId.toInt() >= 0)
    {
        filterString = "LEFT JOIN menu_recipe MR "
                "ON M.id = MR.menu "
                "WHERE MR.recipe = :filterId";
    }

    query.prepare("SELECT M.id, M.name, MP.min_price, MP.avg_price, MP.max_price, C.name 'cat', SC.name 'subcat' "
                  "FROM menu M "
                  "JOIN C_menu_price MP "
                  "ON M.id = MP.id "
                  "LEFT JOIN menu_cat C "
                  "ON C.id = M.cat "
                  "LEFT JOIN menu_subcat SC "
                  "ON SC.id = M.subcat " +
                  filterString
                  );
    if (!filterString.isEmpty())
    {
        query.bindValue(":filterId", menuFilterId.toInt());
    }
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
    compDialog->ui->sta_menu->setModel(rootModel);
}

void MainWindow::sta_loadMenuFilter()
{
    QComboBox *rec;
    rec = compDialog->ui->sta_menu_filter;

    rec->blockSignals(true);
    rec->clear();

    QSqlQuery query;
    query.prepare("SELECT id, name FROM recipe ");
    query.exec();
    rec->addItem(tr("Any"), QVariant::fromValue(-1));
    int selected = 0, counter = 0;
    while (query.next())
    {
        QVariant id = QVariant::fromValue(query.value("id").toInt());
        rec->addItem(query.value("name").toString(), id);
        counter++;
        if (menuFilterId == id)
        {
            selected = counter;
        }
    }
    rec->blockSignals(false);
    rec->setCurrentIndex(selected);
}

void MainWindow::sta_loadRecipeFilter()
{
    QComboBox *rec;
    rec = compDialog->ui->sta_recipe_filter;

    rec->blockSignals(true);
    rec->clear();

    QSqlQuery query;
    query.prepare("SELECT id, name FROM product ");
    query.exec();
    rec->addItem(tr("Any"), QVariant::fromValue(-1));
    int selected = 0, counter = 0;
    while (query.next())
    {
        QVariant id = QVariant::fromValue(query.value("id").toInt());
        rec->addItem(query.value("name").toString(), id);
        counter++;
        if (recipeFilterId == id)
        {
            selected = counter;
        }
    }
    rec->blockSignals(false);
    rec->setCurrentIndex(selected);
}

void MainWindow::sta_menuFilterSelected(int index)
{
    menuFilterId = compDialog->ui->sta_menu_filter->currentData();
    sta_loadTableMenu();
}

void MainWindow::sta_recipeFilterSelected(int index)
{
    recipeFilterId = compDialog->ui->sta_recipe_filter->currentData();
    sta_loadTableRecipe();
}
