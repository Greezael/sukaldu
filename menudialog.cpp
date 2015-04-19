// TORREMOVE
#include <iostream>
#include <sstream>

#include "menudialog.h"
#include "ui_menudialog.h"

#include "mainwindow.h"

#include <QtSql>
#include <QPushButton>

MenuDialog::MenuDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MenuDialog)
{
    ui->setupUi(this);
}

MenuDialog::MenuDialog(int menuIdentifier, int roleIdentifier, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MenuDialog)
{
    this->menuId = menuIdentifier;
    this->roleId = roleIdentifier;
    ui->setupUi(this);

    this->ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    this->ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

    fillCategoryList();
    QObject::connect(this->ui->menurec_cat,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SLOT(catSelected(int)));
    QObject::connect(this->ui->menurec_subcat,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SLOT(subcatSelected(int)));
    QObject::connect(this, SIGNAL(accepted()), this, SLOT(addRecipe()));
}


void MenuDialog::fillCategoryList()
{
    QComboBox *cat, *subcat;
    cat = this->ui->menurec_cat;
    subcat = this->ui->menurec_subcat;

    cat->blockSignals(true);
    cat->clear();

    QSqlQuery query;
    query.prepare("SELECT id, name FROM recipe_cat");
    query.exec();
    cat->addItem(tr("Any"), QVariant::fromValue(-1));
    while (query.next())
    {
        cat->addItem(query.value("name").toString(), QVariant::fromValue(query.value("id").toInt()));
    }
    cat->setCurrentIndex(0);
    cat->blockSignals(false);
    catSelected(0);
}

void MenuDialog::catSelected(int index)
{
    QComboBox *cat, *subcat;
    cat = this->ui->menurec_cat;
    subcat = this->ui->menurec_subcat;

    subcat->blockSignals(true);
    subcat->clear();

    int catId = cat->itemData(index).toInt();

    if (catId != -1)
    {
        QSqlQuery query;
        query.prepare("SELECT id, name FROM recipe_subcat WHERE cat = :catId");
        query.bindValue(":catId", QVariant::fromValue(catId));
        query.exec();
        subcat->addItem(tr("Any"), QVariant::fromValue(-1));
        while (query.next())
        {
            subcat->addItem(query.value("name").toString(), QVariant::fromValue(query.value("id").toInt()));
        }
        subcat->setCurrentIndex(0);
    }
    else
    {
        subcat->addItem(tr("Any"), QVariant::fromValue(-1));
    }
    subcat->blockSignals(false);
    subcatSelected(0);
}

void MenuDialog::subcatSelected(int index)
{
    QComboBox *cat, *subcat, *rec;
    cat = this->ui->menurec_cat;
    subcat = this->ui->menurec_subcat;
    rec = this->ui->menurec_rec;

    rec->clear();

    QVariant catId = cat->currentData();
    QVariant subcatId = subcat->currentData();

    QString catCond = (catId.toInt() >= 0) ? "WHERE cat = :catId " : "";
    QString subcatCond = ((catId.toInt() >= 0) && (subcatId.toInt() >= 0)) ?
                "AND subcat = :subcatId " : "";

    QSqlQuery query;
    query.prepare("SELECT id, name FROM recipe " +
                  catCond +
                  subcatCond +
                  "EXCEPT "
                  "SELECT R.id, R.name "
                  "FROM recipe R JOIN menu_recipe MR "
                  "WHERE MR.recipe = R.id "
                  "AND MR.menu = :menuId "
                  "AND MR.role = :roleId ");
    query.bindValue(":catId", catId);
    query.bindValue(":subcatId", subcatId);
    query.bindValue(":menuId", this->menuId);
    query.bindValue(":roleId", this->roleId);
    query.exec();
    while (query.next())
    {
        rec->addItem(query.value("name").toString(), QVariant::fromValue(query.value("id").toInt()));
    }
    rec->setCurrentIndex(0);
}

void MenuDialog::addRecipe()
{
    if (!this->ui->menurec_rec->currentData().isNull())
    {
        int recId = this->ui->menurec_rec->currentData().toInt();
        QSqlQuery query;
        query.prepare("INSERT INTO menu_recipe VALUES (:menu, :recipe, :role)");
        query.bindValue(":menu", menuId);
        query.bindValue(":recipe", recId);
        query.bindValue(":role", roleId);
        query.exec();
    }

    MainWindow * mainwindow = (MainWindow *) this->parent();
    mainwindow->reloadMenuOptions();
    mainwindow->updateMenuPrice();
}

MenuDialog::~MenuDialog()
{
    delete ui;
}
