//TOREMOVE
#include <iostream>
#include <sstream>

#include "pricedialog.h"
#include "ui_pricedialog.h"

#include "mainwindow.h"

#include <QtSql>
#include <QPushButton>

PriceDialog::PriceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PriceDialog)
{
    this->prodId = -1;
    ui->setupUi(this);
    QObject::connect(this, SIGNAL(accepted()), this, SLOT(addPrice()));
}

PriceDialog::PriceDialog(int prodIdentifier, int priceIdentifier, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PriceDialog)
{
    ui->setupUi(this);
    this->ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    this->ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

    this->prodId = prodIdentifier;
    this->priceId = priceIdentifier;
    int providerId = -1;

    if (priceId >= 0)
    {
        QSqlQuery query;
        query.prepare("SELECT price, quantity, provider FROM prod_price WHERE id = :id");
        query.bindValue(":id", priceId);
        query.exec();
        if (query.next())
        {
            this->ui->price_price->setValue(query.value("price").toDouble());
            this->ui->price_quantity->setValue(query.value("quantity").toDouble());
            providerId = query.value("provider").toInt();
        }
        else
        {
            this->priceId = -1;
        }
    }

    QSettings settings("Sukaldu-dev", "Sukaldu");
    this->ui->price_price_cur->setText(settings.value("currency").toString());

    QSqlQuery query;
    query.prepare("SELECT prod_meas.name FROM product JOIN prod_meas " \
                  "WHERE product.id = :id " \
                  "AND product.meas = prod_meas.id");
    query.bindValue(":id", prodId);
    query.exec();
    if (query.next())
    {
        this->ui->price_quantity_meas->setText(query.value("name").toString());
    }

    query.prepare("SELECT id, name FROM price_provider");
    query.exec();
    int selected = 0, index = 1;
    this->ui->price_prov->addItem(tr("None"), QVariant::fromValue(-1));
    while (query.next())
    {
        this->ui->price_prov->addItem(query.value("name").toString(),
                                     QVariant::fromValue(query.value("id").toInt()));
        if (query.value("id").toInt() == providerId)
            selected = index;
        index++;
    }
    this->ui->price_prov->setCurrentIndex(selected);

    QObject::connect(this, SIGNAL(accepted()), this, SLOT(addPrice()));
}


PriceDialog::~PriceDialog()
{
    delete ui;
}

void PriceDialog::addPrice()
{
    if (this->prodId < 0)
    {
        return;
    }

    double price = this->ui->price_price->value();
    double quantity = this->ui->price_quantity->value();
    int provId = this->ui->price_prov->currentData().toInt();
    QVariant provV = (provId != -1) ? QVariant::fromValue(provId) : QVariant();

    if (this->priceId < 0)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO prod_price VALUES (NULL, :product, :price, :quantity, :prov, NULL )" );
        query.bindValue(":product", prodId);
        query.bindValue(":price", price);
        query.bindValue(":quantity", quantity);
        query.bindValue(":prov", provV);
        query.exec();
    }
    else
    {
        QSqlQuery query;
        query.prepare("UPDATE prod_price SET "
                      "price = :price, "
                      "quantity = :quantity, "
                      "provider = :prov "
                      "WHERE id = :id");
        query.bindValue(":id", priceId);
        query.bindValue(":price", price);
        query.bindValue(":quantity", quantity);
        query.bindValue(":prov", provV);
        query.exec();
    }
    MainWindow * mainwindow = (MainWindow *) this->parent();
    mainwindow->updatePriceList();
    mainwindow->sta_loadTables();
}
