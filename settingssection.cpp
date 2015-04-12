
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QtSql>
#include <QStandardItemModel>

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
    setSectionSelected(0);
}

void MainWindow::setSectionSelected(int index)
{
    int section = this->ui->set_section->itemData(index).toInt();
    if (currentSection == static_cast<SK_Section>(section))
        return;
    currentSection = static_cast<SK_Section>(section);

    QListView *cat = this->ui->set_cat;
    QListView *subCat = this->ui->set_subcat;

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

void MainWindow::setCatSelected(const QModelIndex &current, const QModelIndex &previous)
{
    QListView *cat = this->ui->set_cat;
    QListView *subCat = this->ui->set_subcat;

    int catId = current.data(SK_IdRole).toInt();

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
