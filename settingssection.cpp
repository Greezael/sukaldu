
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QtSql>
#include <QStandardItemModel>
#include <QInputDialog>

// Helpers
QString getPrefix(SK_Section section);

void MainWindow::initSettingsPanel()
{
    QComboBox *sections;

    sections = this->ui->set_section;

    sections->blockSignals(true);
    sections->clear();

    sections->addItem(tr("Ingredients"), SK_S_PROD);
    sections->addItem(tr("Recipes"), SK_S_REC);
    sections->addItem(tr("Menus"), SK_S_MENU);

    sections->setCurrentIndex(0);
    sections->blockSignals(false);

    currentSection = SK_S_NONE;
    currentCategory = -1;
    set_sectionSelected(0);

    resetMeasInfo();
    resetProvInfo();
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
                  "VALUES (NULL, :newcattext)");
    query.bindValue(":newcattext", tr("New Category"));
    query.exec();

    resetCategoriesInfo();
}

void MainWindow::set_addSubCat()
{
    if (currentCategory == -1) return;
    QSqlQuery query;
    query.prepare("INSERT INTO " + getPrefix(currentSection) + "_subcat "
                  "VALUES (NULL, :cat, :newsubcattext)");
    query.bindValue(":newsubcattext", tr("New Subcategory"));
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
    QListView * list = this->ui->set_cat;
    if (list->selectionModel()->selectedIndexes().empty()) return;
    QVariant id = list->selectionModel()->selectedIndexes().first().data(SK_IdRole);
    QString currentName = list->selectionModel()->selectedIndexes().first().data(Qt::DisplayRole).toString();

    bool ok;
    QString newName = QInputDialog::getText(this, tr("Rename Category"),
                                         tr("Category name:"), QLineEdit::Normal,
                                         currentName, &ok);
    if (ok && !newName.isEmpty())
    {
        QString prefix = getPrefix(currentSection);

        QSqlQuery query;
        query.prepare("UPDATE " + prefix + "_cat SET name = :name WHERE id = :id");
        query.bindValue(":id", id);
        query.bindValue(":name", newName);
        query.exec();

        resetCategoriesInfo();
    }
}

void MainWindow::set_renameSubCat()
{
    QListView * list = this->ui->set_subcat;
    if (list->selectionModel()->selectedIndexes().empty()) return;
    QVariant id = list->selectionModel()->selectedIndexes().first().data(SK_IdRole);
    QString currentName = list->selectionModel()->selectedIndexes().first().data(Qt::DisplayRole).toString();

    bool ok;
    QString newName = QInputDialog::getText(this, tr("Rename Subcategory"),
                                         tr("Subcategory name:"), QLineEdit::Normal,
                                         currentName, &ok);
    if (ok && !newName.isEmpty())
    {
        QString prefix = getPrefix(currentSection);

        QSqlQuery query;
        query.prepare("UPDATE " + prefix + "_subcat SET name = :name WHERE id = :id");
        query.bindValue(":id", id);
        query.bindValue(":name", newName);
        query.exec();

        resetSubCategoriesInfo();
    }
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

void MainWindow::resetMeasInfo()
{
    QListView *meas = this->ui->set_meas;

    QStandardItemModel * model;
    if (meas->model() != nullptr)
    {
        model = (QStandardItemModel *) meas->model();
        model->clear();
    }
    else
    {
        model = new QStandardItemModel();
    }

    QSqlQuery query;
    query.prepare("SELECT id, name FROM prod_meas");
    query.exec();
    while (query.next())
    {
        QStandardItem * item = new QStandardItem(query.value("name").toString());
        item->setData(QVariant::fromValue(query.value("id").toInt()), SK_IdRole);
        model->appendRow(item);
    }

    meas->setModel(model);
}

void MainWindow::set_addMeas()
{
    QSqlQuery query;
    query.prepare("INSERT INTO prod_meas "
                  "VALUES (NULL, :new)");
    query.bindValue(":new", tr("New"));
    query.exec();

    resetMeasInfo();
}

void MainWindow::set_deleteMeas()
{
    QListView * list = this->ui->set_meas;
    if (list->selectionModel()->selectedIndexes().empty()) return;
    QVariant id = list->selectionModel()->selectedIndexes().first().data(SK_IdRole);

    QSqlQuery query;
    query.prepare("DELETE FROM prod_meas "
                  "WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    resetMeasInfo();
}
void MainWindow::set_renameMeas()
{
    QListView * list = this->ui->set_meas;
    if (list->selectionModel()->selectedIndexes().empty()) return;
    QVariant id = list->selectionModel()->selectedIndexes().first().data(SK_IdRole);
    QString currentName = list->selectionModel()->selectedIndexes().first().data(Qt::DisplayRole).toString();

    bool ok;
    QString newName = QInputDialog::getText(this, tr("Rename Unit"),
                                         tr("Unit abbreviation:"), QLineEdit::Normal,
                                         currentName, &ok);
    if (ok && !newName.isEmpty())
    {
        QSqlQuery query;
        query.prepare("UPDATE prod_meas SET name = :name WHERE id = :id");
        query.bindValue(":id", id);
        query.bindValue(":name", newName);
        query.exec();

        resetMeasInfo();
    }
}

void MainWindow::resetProvInfo()
{
    QListView *list= this->ui->set_prov;

    QStandardItemModel * model;
    if (list->model() != nullptr)
    {
        model = (QStandardItemModel *) list->model();
        model->clear();
    }
    else
    {
        model = new QStandardItemModel();
    }

    QSqlQuery query;
    query.prepare("SELECT id, name FROM price_provider");
    query.exec();
    while (query.next())
    {
        QStandardItem * item = new QStandardItem(query.value("name").toString());
        item->setData(QVariant::fromValue(query.value("id").toInt()), SK_IdRole);
        model->appendRow(item);
    }

    list->setModel(model);
}

void MainWindow::set_addProv()
{
    QSqlQuery query;
    query.prepare("INSERT INTO price_provider "
                  "VALUES (NULL, :new)");
    query.bindValue(":new", tr("New"));
    query.exec();

    resetProvInfo();
}

void MainWindow::set_deleteProv()
{
    QListView * list = this->ui->set_prov;
    if (list->selectionModel()->selectedIndexes().empty()) return;
    QVariant id = list->selectionModel()->selectedIndexes().first().data(SK_IdRole);

    QSqlQuery query;
    query.prepare("DELETE FROM price_provider "
                  "WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    resetProvInfo();
}
void MainWindow::set_renameProv()
{
    QListView * list = this->ui->set_prov;
    if (list->selectionModel()->selectedIndexes().empty()) return;
    QVariant id = list->selectionModel()->selectedIndexes().first().data(SK_IdRole);
    QString currentName = list->selectionModel()->selectedIndexes().first().data(Qt::DisplayRole).toString();

    bool ok;
    QString newName = QInputDialog::getText(this, tr("Rename Provider"),
                                         tr("Name:"), QLineEdit::Normal,
                                         currentName, &ok);
    if (ok && !newName.isEmpty())
    {
        QSqlQuery query;
        query.prepare("UPDATE price_provider SET name = :name WHERE id = :id");
        query.bindValue(":id", id);
        query.bindValue(":name", newName);
        query.exec();

        resetProvInfo();
    }
}

void MainWindow::set_changeAppLanguage()
{
    QSettings settings("Sukaldu-dev", "Sukaldu");

    QStringList items;
    QStringList codes;
    items << "Castellano" << "English";
    codes << "es" << "en";

    bool ok;
    QString item = QInputDialog::getItem(nullptr, "QInputDialog::getItem()",
                                         "Language", items, 0, false, &ok);

    if (ok && !item.isEmpty())
    {
        for (int i = 0; i < items.length(); i++)
        {
            if (item.compare(items.at(i)) == 0)
            {
                QString localeString = codes.at(i);
                if (settings.value("locale").toString().compare(localeString) != 0)
                {
                    settings.setValue("locale", localeString);
                    settings.sync();
                    QProcess::startDetached(QApplication::applicationFilePath());
                    exit(12);
                }
                break;
            }
        }
    }
}
