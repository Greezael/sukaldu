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

PriceDialog::~PriceDialog()
{
    delete ui;
}

void PriceDialog::addPrice()
{
    double price = this->ui->price_price->value();
    double quantity = this->ui->price_quantity->value();
    QString notes = this->ui->price_notes->toPlainText();

    QSqlQuery query;
    query.prepare("INSERT INTO prod_price VALUES (NULL, :product, :price, :quantity, :notes )" );
    query.bindValue(":product", prodId);
    query.bindValue(":price", price);
    query.bindValue(":quantity", quantity);
    query.bindValue(":notes", notes);
    query.exec();

    MainWindow * mainwindow = (MainWindow *) this->parent();
    mainwindow->updatePriceList();
}
