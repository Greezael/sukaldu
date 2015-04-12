
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QtSql>
#include <QStandardItemModel>

// Helpers
QString getPrefix(SK_Section section);

void MainWindow::initSettingsPanel()
{
    QComboBox *sections;

    sections = this->ui->set_section;

    sections->blockSignals(true);
    sections->clear();

    sections->addItem("Product", SK_S_PROD);
    sections->addItem("Recipe", SK_S_REC);
    sections->addItem("Menu", SK_S_MENU);

    sections->setCurrentIndex(0);
    sections->blockSignals(false);

    currentSection = SK_S_NONE;
    currentCategory = -1;
    set_sectionSelected(0);
}

void MainWindow::rebuildTrees()
{
    buildProductTree();
    buildRecipeTree();
    buildMenuTree();
}

void MainWindow::resetCategoriesInfo()
{
    currentSection = SK_S_NONE;
    set_sectionSelected(currentSectionIndex);
    rebuildTrees();
}

void MainWindow::resetSubCategoriesInfo()
{
    rebuildTrees();
    if (currentCategory != -1)
    {
        set_catSelectedById(currentCategory);
    }
}


void MainWindow::set_sectionSelected(int index)
{
    int section = this->ui->set_section->itemData(index).toInt();
    if (currentSection == static_cast<SK_Section>(section))
        return;
    currentSection = static_cast<SK_Section>(section);
    currentSectionIndex = index;
    currentCategory = -1;

    QListView *cat = this->ui->set_cat;
    QListView *subCat = this->ui->set_subcat;

    QString prefix = getPrefix(currentSection);

    QStandardItemModel * model;
    if (cat->model() != nullptr)
    {
        model = (QStandardItemModel *) cat->model();
        model->clear();
    }
    else
    {
        model = new QStandardItemModel();
    }
    if (subCat->model() != nullptr)
    {
        ((QStandardItemModel *) subCat->model())->clear();
    }

    QSqlQuery query;
    query.prepare("SELECT id, name FROM " + prefix + "_cat");
    query.exec();
    while (query.next())
    {
        QStandardItem * item = new QStandardItem(query.value("name").toString());
        item->setData(QVariant::fromValue(query.value("id").toInt()), SK_IdRole);
        model->appendRow(item);
    }

    cat->setModel(model);
}

void MainWindow::set_catSelected(const QModelIndex &current, const QModelIndex &previous)
{
    set_catSelectedById(current.data(SK_IdRole).toInt());
}

void MainWindow::set_catSelectedById(int id)
{
    QListView *cat = this->ui->set_cat;
    QListView *subCat = this->ui->set_subcat;

    int catId = id;
    currentCategory = catId;

    QString prefix;
    switch (currentSection)
    {
    case SK_S_PROD:
        prefix = "prod";
        break;
    case SK_S_REC:
        prefix = "recipe";
        break;
    case SK_S_MENU:
        prefix = "menu";
        break;
    }

    QStandardItemModel * model;
    if (subCat->model() != nullptr)
    {
        model = (QStandardItemModel *) subCat->model();
        model->clear();
    }
    else
    {
        model = new QStandardItemModel();
    }


    QSqlQuery query;
    query.prepare("SELECT id, name FROM " + prefix + "_subcat "
                  "WHERE cat = :catId");
    query.bindValue(":catId", catId);
    query.exec();
    while (query.next())
    {
        QStandardItem * item = new QStandardItem(query.value("name").toString());
        item->setData(QVariant::fromValue(query.value("id").toInt()), SK_IdRole);
        model->appendRow(item);
    }

    subCat->setModel(model);
}

void MainWindow::set_addCat()
{
    QSqlQuery query;
    query.prepare("INSERT INTO " + getPrefix(currentSection) + "_cat "
                  "VALUES (NULL, 'New category')");
    query.exec();

    resetCategoriesInfo();
}

void MainWindow::set_addSubCat()
{
    if (currentCategory == -1) return;
    QSqlQuery query;
    query.prepare("INSERT INTO " + getPrefix(currentSection) + "_subcat "
                  "VALUES (NULL, :cat, 'New subcategory')");
    query.bindValue(":cat", currentCategory);
    query.exec();

    resetSubCategoriesInfo();
}

void MainWindow::set_deleteCat()
{
    QListView * list = this->ui->set_cat;
    if (list->selectionModel()->selectedIndexes().empty()) return;
    QVariant id = list->selectionModel()->selectedIndexes().first().data(SK_IdRole);

    QString prefix = getPrefix(currentSection);

    QSqlQuery query;
    query.prepare("DELETE FROM " + prefix + "_cat "
                  "WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    resetCategoriesInfo();
}

void MainWindow::set_deleteSubCat()
{
    QListView * list = this->ui->set_subcat;
    if (list->selectionModel()->selectedIndexes().empty()) return;
    QVariant id = list->selectionModel()->selectedIndexes().first().data(SK_IdRole);

    QString prefix = getPrefix(currentSection);

    QSqlQuery query;
    query.prepare("DELETE FROM " + prefix + "_subcat "
                  "WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    resetSubCategoriesInfo();
}

void MainWindow::set_renameCat()
{
}

void MainWindow::set_renameSubCat()
{

}

QString getPrefix(SK_Section section)
{
    QString prefix;
    switch (section)
    {
    case SK_S_PROD:
        prefix = "prod";
        break;
    case SK_S_REC:
        prefix = "recipe";
        break;
    case SK_S_MENU:
        prefix = "menu";
        break;
    }
    return prefix;
}
