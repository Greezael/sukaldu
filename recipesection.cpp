#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QtSql>
#include <QStandardItemModel>

void MainWindow::buildRecipeTree()
{
    buildTree(SK_S_REC);
}

void MainWindow::recipeSelected(int id)
{
    this->currentRecipe = id;
    if (id == -1)
    {
        this->ui->rec_scroll->setEnabled(false);
        this->ui->rec_name->clear();
        this->ui->rec_cat->clear();
        this->ui->rec_subcat->clear();
        if (this->ui->rec_ing->model() != NULL)
        {
            ((QStandardItemModel *) this->ui->rec_ing->model())->clear();
        }
        this->ui->rec_elab->clear();
        this->ui->rec_servs->clear();
        return;
    }

    this->ui->rec_scroll->setEnabled(true);

    QSqlQuery query;
    query.prepare("SELECT name, cat, subcat, preparation, servings "
                  "FROM recipe "
                  "WHERE id = :id" );
    query.bindValue(":id", id);
    query.exec();
    if (query.next())
    {
        this->ui->rec_name->setText(query.value("name").toString());
        this->ui->rec_elab->setPlainText(query.value("preparation").toString());
        int cat = -1;
        int subcat = -1;
        int servs = -1;
        cat = (query.value("cat").isNull()) ? -1 : query.value("cat").toInt();
        subcat = (query.value("subcat").isNull()) ? -1 : query.value("subcat").toInt();
        servs = (query.value("servings").isNull()) ? 0 : query.value("servings").toInt();
        this->ui->rec_servs->setValue(servs);
        fillRecipeCategoryLists(cat, subcat);
//        updatePriceList();
    }
}

void MainWindow::fillRecipeCategoryLists(int catId, int subCatId)
{
    fillCategoryLists(catId, subCatId, SK_S_REC);
}


void MainWindow::recCatSelected(int index)
{
    catSelected(index, SK_S_REC);
}
