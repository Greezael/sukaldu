#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QtSql>
#include <QStandardItemModel>

#include "menudialog.h"

void MainWindow::buildMenuTree()
{
    buildTree(SK_S_MENU);
}

void MainWindow::menuSelected(int id)
{
    cleanMenuOptions();
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
        fillMenuOptions();
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

void MainWindow::fillMenuOptions()
{
    cleanMenuOptions();

    menuOptions.clear();

    QSqlQuery query;
    query.prepare("SELECT id "
                  "FROM menu_role "
                  "WHERE menu = :id" );
    query.bindValue(":id", currentMenu);
    query.exec();
    firstMenuOptionRow = -1;
    while (query.next())
    {
        QVariant optionId = query.value("id");
        menuOptions.push_back(optionId);
        showMenuOption(optionId);
    }

    // Add option button
    QFormLayout *layout = (QFormLayout *) this->ui->menu_scroll_contents->layout();
    QHBoxLayout *buttonBox = new QHBoxLayout();
    QSpacerItem *spacer = new QSpacerItem(1, 1, QSizePolicy::Expanding);
    QPushButton *addButton = new QPushButton("Add Option");
    QObject::connect(addButton, &QPushButton::clicked, [=]() {this->addOption();});

    buttonBox->addSpacerItem(spacer);
    buttonBox->addWidget(addButton);

    layout->addRow(buttonBox);

    int row;
    QFormLayout::ItemRole role;
    layout->getLayoutPosition(buttonBox, &row, &role);
    if (firstMenuOptionRow <= 0) firstMenuOptionRow = row;
}

void MainWindow::reloadMenuOptions()
{
    cleanMenuOptions();
    fillMenuOptions();
}

void MainWindow::showMenuOption(QVariant roleid)
{
    QFormLayout *layout = (QFormLayout *) this->ui->menu_scroll_contents->layout();

    QLabel *optionLabel = new QLabel("Option");
    QTableView *tableView = new QTableView();
    QHBoxLayout *buttonBox = new QHBoxLayout();
    QSpacerItem *spacer = new QSpacerItem(1, 1, QSizePolicy::Expanding);
    QPushButton *remButton = new QPushButton("Remove");
    QPushButton *addButton = new QPushButton("Add");
    QPushButton *remOptButton = new QPushButton("Remove Option");
    QPushButton *renOptButton = new QPushButton("Rename Option");

    layout->addRow(optionLabel, tableView);

    buttonBox->addWidget(remOptButton);
    buttonBox->addWidget(renOptButton);
    buttonBox->addSpacerItem(spacer);
    buttonBox->addWidget(remButton);
    buttonBox->addWidget(addButton);

    layout->addRow(buttonBox);

    int row;
    QFormLayout::ItemRole role;

    layout->getWidgetPosition(optionLabel, &row, &role);
    if (firstMenuOptionRow <= 0)
    {
        firstMenuOptionRow = row;
    }

    QObject::connect(addButton, &QPushButton::clicked, [=]() {this->showAddRecipePopup(row);});
    QObject::connect(remButton, &QPushButton::clicked, [=]() {this->removeRecipes(row);});

    QObject::connect(remOptButton, &QPushButton::clicked, [=]() {this->removeOption(row);});
    QObject::connect(renOptButton, &QPushButton::clicked, [=]() {this->renameOption(row);});

    QStandardItemModel * rootModel = new QStandardItemModel(this);

    QSqlQuery query;

    query.prepare("SELECT MR.recipe, R.name "
                  "FROM menu_recipe MR "
                  "JOIN recipe R "
                  "WHERE MR.menu = :menuid "
                  "AND R.id = MR.recipe "
                  "AND MR.role = :role");
    query.bindValue(":menuid", currentMenu);
    query.bindValue(":role", roleid);
    query.exec();
    rootModel->setColumnCount(1);
    while (query.next())
    {
        QList<QStandardItem*> row;
        QStandardItem* recipeName = new QStandardItem(query.value("name").toString());
        row << recipeName;
        recipeName->setData(query.value("recipe"), SK_IdRole);
        rootModel->appendRow(row);
    }

    tableView->setModel(rootModel);

    query.prepare("SELECT name "
                  "FROM menu_role "
                  "WHERE id = :role");
    query.bindValue(":role", roleid);
    query.exec();
    if (query.next())
    {
        optionLabel->setText(query.value("name").toString());
    }
}

void MainWindow::cleanMenuOptions()
{
    if (firstMenuOptionRow <= 0) return;

    QFormLayout *layout = (QFormLayout *) this->ui->menu_scroll_contents->layout();

    // The +1 is for the add option button at the end
    for (int i = 0; i < (int) menuOptions.size() * 2 + 1; i++)
    {
        int row = firstMenuOptionRow + i;
        QLayoutItem * field = layout->itemAt(row, QFormLayout::FieldRole);
        if (field != nullptr)
        {
            if (QLayout * boxLayout = dynamic_cast<QLayout *>(field->layout()))
            {
                for (int j = 0; j < boxLayout->count(); j++)
                {
                    if (QWidget * widget = (boxLayout->itemAt(j)->widget()))
                    {
                        widget->deleteLater();
                    }
                }
                boxLayout->deleteLater();
            }
            else
            {
                field->widget()->hide();
                field->widget()->deleteLater();
            }
            layout->removeItem(field);
        }
        QLayoutItem * label = layout->itemAt(row, QFormLayout::LabelRole);
        if (label != nullptr)
        {
            label->widget()->hide();
            label->widget()->deleteLater();
            layout->removeItem(label);
        }
    }

    menuOptions.clear();
    this->ui->menu_scroll_contents->setLayout(layout);
    this->ui->menu_scroll_contents->update();
}


void MainWindow::showAddRecipePopup(int row)
{
    int index = (row - firstMenuOptionRow) / 2;
    QVariant roleId = menuOptions.at(index);
    MenuDialog* popUp = new MenuDialog(currentMenu, roleId.toInt(), this);
    popUp->setModal(true);
    popUp->show();
}

void MainWindow::removeRecipes(int row)
{
    int index = (row - firstMenuOptionRow) / 2;
    QVariant roleId = menuOptions.at(index);

    QFormLayout *layout = (QFormLayout *) this->ui->menu_scroll_contents->layout();

    QLayoutItem * field = layout->itemAt(row, QFormLayout::FieldRole);
    QTableView * productTable = nullptr;
    if ((field != nullptr) && (productTable = dynamic_cast<QTableView *>(field->widget())))
    {
        QModelIndexList indexes = productTable->selectionModel()->selectedIndexes();
        bool deleted = false;

        for (int i = 0; i < indexes.size(); i++)
        {
            QModelIndex tableIndex = indexes.at(i);
            QVariant recipeId = productTable->model()->itemData(tableIndex)[SK_IdRole];
            QSqlQuery query;
            query.prepare("DELETE FROM menu_recipe "
                          "WHERE menu = :menuId "
                          "AND recipe = :recId "
                          "AND role = :roleId");
            query.bindValue(":menuId", this->currentMenu);
            query.bindValue(":recId", recipeId);
            query.bindValue(":roleId", roleId);
            query.exec();
            if (query.lastError().type() == QSqlError::NoError)
            {
                deleted = true;
            }
        }
        if (deleted)
        {
            reloadMenuOptions();
        }
    }
}

void MainWindow::addOption()
{
    QSqlQuery query;
    query.prepare("INSERT INTO menu_role VALUES (NULL, :menuId, 'Unnamed') ");
    query.bindValue(":menuId", this->currentMenu);
    query.exec();
    if (query.lastError().type() != QSqlError::NoError)
    {
        std::cout << query.lastError().text().toStdString() << std::endl;
    }
    this->reloadMenuOptions();
}

void MainWindow::removeOption(int row)
{
    int index = (row - firstMenuOptionRow) / 2;
    QVariant roleId = menuOptions.at(index);
    QSqlQuery query;
    query.prepare("DELETE FROM menu_role "
                  "WHERE id = :roleId");
    query.bindValue(":roleId", roleId);
    query.exec();

    this->reloadMenuOptions();
}


void MainWindow::renameOption(int row)
{
}
