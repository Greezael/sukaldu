//TOREMOVE
#include <iostream>
#include <sstream>

#include "pricedialog.h"
#include "ui_pricedialog.h"

#include "mainwindow.h"

#include <QtSql>

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
    this->prodId = prodIdentifier;
    this->priceId = priceIdentifier;
    if (priceId >= 0)
    {
        QSqlQuery query;
        query.prepare("SELECT price, quantity, notes FROM prod_price WHERE id = :id");
        query.bindValue(":id", priceId);
        query.exec();
        if (query.next())
        {
            this->ui->price_price->setValue(query.value("price").toDouble());
            this->ui->price_quantity->setValue(query.value("quantity").toDouble());
            this->ui->price_notes->setText(query.value("notes").toString());
        }
        else
        {
            this->priceId = -1;
        }
    }

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
    QString notes = this->ui->price_notes->toPlainText();

    if (this->priceId < 0)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO prod_price VALUES (NULL, :product, :price, :quantity, :notes )" );
        query.bindValue(":product", prodId);
        query.bindValue(":price", price);
        query.bindValue(":quantity", quantity);
        query.bindValue(":notes", notes);
        query.exec();
    }
    else
    {
        QSqlQuery query;
        query.prepare("UPDATE prod_price SET "
                      "price = :price, "
                      "quantity = :quantity, "
                      "notes = :notes "
                      "WHERE id = :id");
        query.bindValue(":id", priceId);
        query.bindValue(":price", price);
        query.bindValue(":quantity", quantity);
        query.bindValue(":notes", notes);
        query.exec();
    }
    MainWindow * mainwindow = (MainWindow *) this->parent();
    mainwindow->updatePriceList();
}
