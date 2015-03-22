#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QtSql>
#include <QStandardItemModel>

void MainWindow::buildMenuTree()
{
    buildTree(SK_S_MENU);
}

void MainWindow::menuSelected(int id)
{
    this->currentMenu = id;
    if (id == -1)
    {
        this->ui->menu_scroll->setEnabled(false);
        this->ui->menu_name->clear();
        this->ui->menu_cat->clear();
        this->ui->menu_subcat->clear();
        this->ui->menu_price->setText("--");
        return;
    }

    this->ui->menu_scroll->setEnabled(true);

    QSqlQuery query;
    query.prepare("SELECT name, cat, subcat "
                  "FROM menu "
                  "WHERE id = :id" );
    query.bindValue(":id", id);
    query.exec();
    if (query.next())
    {
        this->ui->menu_name->setText(query.value("name").toString());
        int cat = -1;
        int subcat = -1;
        cat = (query.value("cat").isNull()) ? -1 : query.value("cat").toInt();
        subcat = (query.value("subcat").isNull()) ? -1 : query.value("subcat").toInt();
        fillMenuCategoryLists(cat, subcat);
        updateMenuPrice();
    }
}

void MainWindow::updateMenuPrice()
{
    if (this->currentMenu < 0)
    {
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT price "
                  "FROM C_menu_price "
                  "WHERE id = :id" );
    query.bindValue(":id", this->currentMenu);
    query.exec();
    if (query.next())
    {
        if (!query.value("price").isNull())
        {
            this->ui->menu_price->setText(query.value("price").toString());
            return;
        }
    }
    this->ui->menu_price->setText("--");
}

void MainWindow::fillMenuCategoryLists(int catId, int subCatId)
{
    fillCategoryLists(catId, subCatId, SK_S_MENU);
}

void MainWindow::menuCatSelected(int index)
{
    catSelected(index, SK_S_MENU);
}

void MainWindow::saveMenuData()
{
    QString name = this->ui->menu_name->text();
    int catId = this->ui->menu_cat->currentData().toInt();
    int subCatId = this->ui->menu_subcat->currentData().toInt();
    QVariant catV = (catId != -1) ? QVariant::fromValue(catId) : QVariant();
    QVariant subCatV = (subCatId != -1) ? QVariant::fromValue(subCatId) : QVariant();

    QSqlQuery query;
    query.prepare("UPDATE menu SET "
                  "name = :name, "
                  "cat = :cat, "
                  "subcat = :subcat "
                  "WHERE id = :id");
    query.bindValue(":name", name);
    query.bindValue(":cat", catV);
    query.bindValue(":subcat", subCatV);
    query.bindValue(":id", currentMenu);
    query.exec();

    menuSelected(currentMenu);
    buildMenuTree();
}

void MainWindow::generalMenuButtonClicked(QAbstractButton *button)
{
    switch (this->ui->menu_curmenu_buttons->standardButton(button))
    {
    case QDialogButtonBox::Reset:
        resetMenuData();
        break;
    case QDialogButtonBox::Save:
        saveMenuData();
        break;
    default:
        break;
    }
}

void MainWindow::resetMenuData()
{
    menuSelected(currentMenu);
}

