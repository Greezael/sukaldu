// TORREMOVE
#include <iostream>
#include <sstream>

#include "recipedialog.h"
#include "ui_recipedialog.h"

#include "mainwindow.h"

#include <QtSql>

RecipeDialog::RecipeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecipeDialog)
{
    ui->setupUi(this);
}

RecipeDialog::RecipeDialog(int recIdentifier, int prodIdentifier, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecipeDialog)
{
    ui->setupUi(this);
    this->recId = recIdentifier;
    this->prodId = prodIdentifier;
    if (prodId >= 0)
    {
        QSqlQuery query;
        query.prepare("SELECT C.name 'cat', S.name 'subcat', P.name, RP.quantity "
                      "FROM recipe_product RP JOIN product P "
                      "JOIN prod_cat C JOIN prod_subcat S "
                      "WHERE RP.product = :prodid "
                      "AND RP.recipe = :recid "
                      "AND RP.product = P.id "
                      "AND P.cat = C.id "
                      "AND P.subcat = S.id");
        query.bindValue(":prodid", prodId);
        query.bindValue(":recid", recId);
        query.exec();
        if (query.lastError().type() != QSqlError::NoError)
        {
            std::cerr << "ERROR: " << query.lastError().text().toUtf8().constData() << std::endl;
            std::cerr << "--------" << std::endl;
            std::exit(-1);
        }
        if (query.next())
        {
            // Cat
            if (query.value("cat").isNull())
                this->ui->recprod_cat->addItem("None");
            else
                this->ui->recprod_cat->addItem(query.value("cat").toString());
            this->ui->recprod_cat->setEnabled(false);
            // Subcat
            if (query.value("subcat").isNull())
                this->ui->recprod_subcat->addItem("None");
            else
                this->ui->recprod_subcat->addItem(query.value("subcat").toString());
            this->ui->recprod_subcat->setEnabled(false);
            // Product
            this->ui->recprod_prod->addItem(query.value("name").toString());
            this->ui->recprod_prod->setEnabled(false);
            // Quantity
            this->ui->recprod_quantity->setValue(query.value("quantity").toDouble());
        }
        else
        {
            this->prodId = -1;
        }
    }

    if (this->prodId < 0)
    {
        fillCategoryList();
        QObject::connect(this->ui->recprod_cat,
                         SIGNAL(currentIndexChanged(int)),
                         this,
                         SLOT(catSelected(int)));
        QObject::connect(this->ui->recprod_subcat,
                         SIGNAL(currentIndexChanged(int)),
                         this,
                         SLOT(subcatSelected(int)));
    }

    QObject::connect(this, SIGNAL(accepted()), this, SLOT(addRecipe()));
}

void RecipeDialog::fillCategoryList()
{
    QComboBox *cat, *subcat;
    cat = this->ui->recprod_cat;
    subcat = this->ui->recprod_subcat;

    cat->blockSignals(true);
    cat->clear();

    QSqlQuery query;
    query.prepare("SELECT id, name FROM prod_cat");
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

void RecipeDialog::catSelected(int index)
{
    QComboBox *cat, *subcat;
    cat = this->ui->recprod_cat;
    subcat = this->ui->recprod_subcat;

    subcat->blockSignals(true);
    subcat->clear();

    int catId = cat->itemData(index).toInt();

    if (catId != -1)
    {
        QSqlQuery query;
        query.prepare("SELECT id, name FROM prod_subcat WHERE cat = :catId");
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

void RecipeDialog::subcatSelected(int index)
{
    QComboBox *cat, *subcat, *prod;
    cat = this->ui->recprod_cat;
    subcat = this->ui->recprod_subcat;
    prod = this->ui->recprod_prod;

    prod->clear();

    QVariant catId = cat->currentData();
    QVariant subcatId = subcat->currentData();

    QString catCond = (catId.toInt() >= 0) ? "WHERE cat = :catId " : "";
    QString subcatCond = ((catId.toInt() >= 0) && (subcatId.toInt() >= 0)) ?
                "AND subcat = :subcatId " : "";

    QSqlQuery query;
    query.prepare("SELECT id, name FROM product " +
                  catCond +
                  subcatCond +
                  "EXCEPT "
                  "SELECT P.id, P.name "
                  "FROM product P JOIN recipe_product RP "
                  "WHERE P.id = RP.product "
                  "AND RP.recipe = :recId");
    query.bindValue(":catId", catId);
    query.bindValue(":subcatId", subcatId);
    query.bindValue(":recId", recId);
    query.exec();
    while (query.next())
    {
        prod->addItem(query.value("name").toString(), QVariant::fromValue(query.value("id").toInt()));
    }
    prod->setCurrentIndex(0);
}

RecipeDialog::~RecipeDialog()
{
    delete ui;
}

void RecipeDialog::addRecipe()
{
    if (this->prodId >= 0)
    {
        float quantity = this->ui->recprod_quantity->value();
        QSqlQuery query;
        query.prepare("UPDATE recipe_product SET "
                      "quantity = :quantity "
                      "WHERE product = :product "
                      "AND recipe = :recipe");
        query.bindValue(":quantity", quantity);
        query.bindValue(":recipe", this->recId);
        query.bindValue(":product", this->prodId);
        query.exec();
    }
    else
    {
        if (!this->ui->recprod_prod->currentData().isNull())
        {
            int prodId = this->ui->recprod_prod->currentData().toInt();
            float quantity = this->ui->recprod_quantity->value();
            QSqlQuery query;
            query.prepare("INSERT INTO recipe_product VALUES (:recipe, :product, :quantity)");
            query.bindValue(":recipe", this->recId);
            query.bindValue(":product", prodId);
            query.bindValue(":quantity", quantity);
            query.exec();
        }
    }

    MainWindow * mainwindow = (MainWindow *) this->parent();
    mainwindow->updateIngredientsList();
}
