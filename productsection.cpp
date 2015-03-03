#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QtSql>
#include <QStandardItemModel>

#include "pricedialog.h"

void MainWindow::makeProductConnections()
{
    QObject::connect(this->ui->prod_newprice,
                     SIGNAL(clicked()),
                     this,
                     SLOT(showNewPricePopup()));
    QObject::connect(this->ui->prod_setcurrentprice,
                     SIGNAL(clicked()),
                     this,
                     SLOT(setCurrentPrice()));
    QObject::connect(this->ui->prod_curprod_buttons,
                     SIGNAL(clicked(QAbstractButton*)),
                     this,
                     SLOT(generalProdButtonClicked(QAbstractButton*)));
    QObject::connect(this->ui->prod_cat,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SLOT(prodCatSelected(int)));
    QObject::connect(this->ui->prod_newprod,
                     SIGNAL(clicked()),
                     this,
                     SLOT(insertNewProduct()));
}

void MainWindow::buildProductTree()
{
    QStandardItemModel * rootModel;
    if (this->ui->prod_tree->model() != NULL)
    {
        rootModel = (QStandardItemModel *) this->ui->prod_tree->model();
        rootModel->clear();
    }
    else
    {
        rootModel = new QStandardItemModel(this);
    }

    this->ui->prod_tree->blockSignals(true);


    QSqlQuery queryCat;
    QSqlQuery querySubcat;
    QSqlQuery queryProduct;

    queryCat.prepare("SELECT id, name FROM prod_cat");
    queryCat.exec();

    while (queryCat.next())
    {
        QStandardItem * category = new QStandardItem(queryCat.value("name").toString());
        QVariant catId = queryCat.value("id");
        category->setData(QVariant::fromValue((int) SK_Category), SK_TypeRole);
        category->setData(catId, SK_IdRole);
        rootModel->appendRow(category);

        querySubcat.prepare("SELECT id, name FROM prod_subcat "
                            "WHERE cat = :cat");
        querySubcat.bindValue(":cat", catId);
        querySubcat.exec();

        while (querySubcat.next())
        {
            QStandardItem * subcategory = new QStandardItem(querySubcat.value("name").toString());
            QVariant subCatId = querySubcat.value("id");
            subcategory->setData(QVariant::fromValue((int) SK_Subcategory), SK_TypeRole);
            subcategory->setData(subCatId, SK_IdRole);
            category->appendRow(subcategory);

            queryProduct.prepare("SELECT id, name FROM product "
                                 "WHERE cat = :catId AND subcat = :subCatId");
            queryProduct.bindValue(":catId", catId);
            queryProduct.bindValue(":subCatId", subCatId);
            queryProduct.exec();

            while (queryProduct.next())
            {
                QStandardItem * product = new QStandardItem(queryProduct.value("name").toString());
                product->setData(QVariant::fromValue((int) SK_Product), SK_TypeRole);
                product->setData(QVariant::fromValue(queryProduct.value("id").toInt()), SK_IdRole);
                subcategory->appendRow(product);
                if (queryProduct.value("id").toInt() == currentProduct)
                {
                    this->ui->prod_tree->setExpanded(rootModel->indexFromItem(category), true);
                    this->ui->prod_tree->setExpanded(rootModel->indexFromItem(subcategory), true);
                    this->ui->prod_tree->selectionModel()->setCurrentIndex(rootModel->indexFromItem(product),
                                                                           QItemSelectionModel::SelectCurrent);
                }
            }
        }

        queryProduct.prepare("SELECT id, name FROM product "
                             "WHERE cat = :catId AND subcat IS NULL");
        queryProduct.bindValue(":catId", catId);
        queryProduct.exec();

        while (queryProduct.next())
        {
            QStandardItem * product = new QStandardItem(queryProduct.value("name").toString());
            product->setData(QVariant::fromValue((int) SK_Product), SK_TypeRole);
            product->setData(QVariant::fromValue(queryProduct.value("id").toInt()), SK_IdRole);
            category->appendRow(product);
            if (queryProduct.value("id").toInt() == currentProduct)
            {
                this->ui->prod_tree->setExpanded(rootModel->indexFromItem(category), true);
                this->ui->prod_tree->selectionModel()->setCurrentIndex(rootModel->indexFromItem(product),
                                                                       QItemSelectionModel::SelectCurrent);
            }
        }
    }

    queryProduct.prepare("SELECT id, name FROM product "
                         "WHERE cat IS NULL AND subcat IS NULL");
    queryProduct.exec();

    while (queryProduct.next())
    {
        QStandardItem * product = new QStandardItem(queryProduct.value("name").toString());
        product->setData(QVariant::fromValue((int) SK_Product), SK_TypeRole);
        product->setData(QVariant::fromValue(queryProduct.value("id").toInt()), SK_IdRole);
        rootModel->appendRow(product);
        if (queryProduct.value("id").toInt() == currentProduct)
        {
            this->ui->prod_tree->selectionModel()->setCurrentIndex(rootModel->indexFromItem(product),
                                                                   QItemSelectionModel::SelectCurrent);
        }
    }

    this->ui->prod_tree->setModel(rootModel);
    this->ui->prod_tree->blockSignals(false);

}


void MainWindow::prodTreeItemSelected(const QModelIndex &current, const QModelIndex &previous)
{
    int type = current.data(SK_TypeRole).toInt();
    int id = current.data(SK_IdRole).toInt();

    if (type == SK_Product)
    {
        productSelected(id);
    }
}

void MainWindow::productSelected(int id)
{
    this->currentProduct = id;
    this->ui->prod_scroll->setEnabled(true);

    QSqlQuery query;
    query.prepare("SELECT name, notes, cat, subcat, meas "
                  "FROM product "
                  "WHERE id = :id" );
    query.bindValue(":id", id);
    query.exec();
    if (query.next())
    {
        this->ui->prod_name->setText(query.value("name").toString());
        this->ui->prod_notes->setPlainText(query.value("notes").toString());
        int cat = -1;
        int subcat = -1;
        int meas = -1;
        cat = (query.value("cat").isNull()) ? -1 : query.value("cat").toInt();
        subcat = (query.value("subcat").isNull()) ? -1 : query.value("subcat").toInt();
        meas = (query.value("meas").isNull()) ? -1 : query.value("meas").toInt();
        fillProductCategoryLists(cat, subcat);
        fillProductMeasurementList(meas);
        updatePriceList();
    }
}

void MainWindow::updatePriceList()
{
    QStandardItemModel * rootModel;
    if (this->ui->prod_pricetable->model() != NULL)
    {
        rootModel = (QStandardItemModel *) this->ui->prod_pricetable->model();
        rootModel->clear();
    }
    else
    {
        rootModel = new QStandardItemModel(this);
    }

    int currentPrice = -1;

    QSqlQuery query;
    query.prepare("SELECT current_price FROM product WHERE id = :prodid");
    query.bindValue(":prodid", QVariant::fromValue(this->currentProduct));
    query.exec();
    if (query.next())
    {
        currentPrice = query.value("current_price").toInt();
    }

    query.prepare("SELECT id, price / quantity 'cprice', notes FROM prod_price WHERE product = :prodid");
    query.bindValue(":prodid", QVariant::fromValue(this->currentProduct));
    query.exec();
    rootModel->setColumnCount(3);
    while (query.next())
    {
        QList<QStandardItem*> row;
        QStandardItem* price = new QStandardItem(query.value("cprice").toString());
        QStandardItem* notes = new QStandardItem(query.value("notes").toString());
        QStandardItem* current = new QStandardItem("");

        if (query.value("id").toInt() == currentPrice)
        {
            current->setText("✓");
        }
        row << price << notes << current;

        price->setData(query.value("id"), SK_IdRole);

        rootModel->appendRow(row);

    }

    this->ui->prod_pricetable->setModel(rootModel);
}

void MainWindow::fillProductCategoryLists(int catId, int subCatId)
{
    this->currentCatId = catId;
    this->currentSubCatId = subCatId;

    this->ui->prod_cat->blockSignals(true);
    this->ui->prod_cat->clear();

    QSqlQuery query;
    std::stringstream query_str;
    query_str << "SELECT id, name FROM prod_cat" << std::endl;
    query.exec(query_str.str().c_str());
    int selected = 0, index = 1;
    this->ui->prod_cat->addItem("None", QVariant::fromValue(-1));
    while (query.next())
    {
        this->ui->prod_cat->addItem(query.value("name").toString(), QVariant::fromValue(query.value("id").toInt()));
        if (query.value("id").toInt() == catId)
            selected = index;
        index++;
    }
    this->ui->prod_cat->setCurrentIndex(selected);
    this->ui->prod_cat->blockSignals(false);
    prodCatSelected(selected);
}

void MainWindow::prodCatSelected(int index)
{
    this->ui->prod_subcat->clear();
    int catId = this->ui->prod_cat->itemData(index).toInt();
    if (catId != this->currentCatId)
    {
        this->currentCatId = catId;
        this->currentSubCatId = -1;
    }
    int subCatId = this->currentSubCatId;

    if (catId != -1)
    {
        QSqlQuery query;
        std::stringstream query_str;
        query_str << "SELECT id, name FROM prod_subcat" << std::endl;
        query_str << "WHERE cat = " << catId << std::endl;
        query.exec(query_str.str().c_str());
        int selected = 0, index = 1;
        this->ui->prod_subcat->addItem("None", QVariant::fromValue(-1));
        while (query.next())
        {
            this->ui->prod_subcat->addItem(query.value("name").toString(), QVariant::fromValue(query.value("id").toInt()));
            if (query.value("id").toInt() == subCatId)
                selected = index;
            index++;
        }
        this->ui->prod_subcat->setCurrentIndex(selected);
    }
    else
    {
        this->ui->prod_subcat->addItem("None", QVariant::fromValue(-1));
    }
}

void MainWindow::fillProductMeasurementList(int measId)
{
    this->ui->prod_meas->clear();

    QSqlQuery query;
    query.prepare("SELECT id, name FROM prod_meas");
    query.exec();
    int selected = 0, index = 1;
    this->ui->prod_meas->addItem("None", QVariant::fromValue(-1));
    while (query.next())
    {
        this->ui->prod_meas->addItem(query.value("name").toString(),
                                     QVariant::fromValue(query.value("id").toInt()));
        if (query.value("id").toInt() == measId)
            selected = index;
        index++;
    }
    this->ui->prod_meas->setCurrentIndex(selected);
}

void MainWindow::showNewPricePopup()
{
    PriceDialog* popUp = new PriceDialog(this);
    popUp->product(currentProduct);
    popUp->setModal(true);
    popUp->show();
}

void MainWindow::generalProdButtonClicked(QAbstractButton *button)
{
    switch (this->ui->prod_curprod_buttons->standardButton(button))
    {
    case QDialogButtonBox::Reset:
        resetProductData();
        break;
    case QDialogButtonBox::Save:
        saveProductData();
        break;
    default:
        break;
    }
}

void MainWindow::resetProductData()
{
    productSelected(currentProduct);
}

void MainWindow::saveProductData()
{
    QString name = this->ui->prod_name->text();
    QString notes = this->ui->prod_notes->toPlainText();
    int catId = this->ui->prod_cat->currentData().toInt();
    int subCatId = this->ui->prod_subcat->currentData().toInt();
    int measId = this->ui->prod_meas->currentData().toInt();
    QVariant catV = (catId != -1) ? QVariant::fromValue(catId) : QVariant();
    QVariant subCatV = (subCatId != -1) ? QVariant::fromValue(subCatId) : QVariant();
    QVariant measV = (measId != -1) ? QVariant::fromValue(measId) : QVariant();
    std::cout << measId << std::endl;

    QSqlQuery query;
    query.prepare("UPDATE product SET "
                  "name = :name, "
                  "notes = :notes, "
                  "cat = :cat, "
                  "subcat = :subcat, "
                  "meas = :meas "
                  "WHERE id = :id");
    query.bindValue(":name", name);
    query.bindValue(":notes", notes);
    query.bindValue(":cat", catV);
    query.bindValue(":subcat", subCatV);
    query.bindValue(":meas", measV);
    query.bindValue(":id", currentProduct);
    query.exec();

    productSelected(currentProduct);
    buildProductTree();
}

void MainWindow::setCurrentPrice()
{
    QModelIndexList indexes = this->ui->prod_pricetable->selectionModel()->selectedIndexes();
    if (indexes.size())
    {
        QModelIndex tableIndex = indexes.first();
        QVariant priceid = this->ui->prod_pricetable->model()->itemData(tableIndex)[SK_IdRole];
        QSqlQuery query;
        query.prepare("UPDATE product SET current_price = :priceid WHERE id = :id");
        query.bindValue(":id", currentProduct);
        query.bindValue(":priceid", priceid);
        query.exec();

        updatePriceList();
    }
}

void MainWindow::insertNewProduct()
{
    QSqlQuery query;
    query.prepare("INSERT INTO product VALUES ( "
                  "NULL, "  // Id
                  "\"New Product\", "  // Name
                  "NULL, "  // Cat
                  "NULL, "  // Subcat
                  "NULL, "  // Notes
                  "NULL, "  // Price
                  "NULL "  // Measurement
                  ")");
    query.exec();

    query.prepare("SELECT last_insert_rowid()");
    query.exec();
    if (query.next())
    {
        productSelected(query.value(0).toInt());
    }
    buildProductTree();
}
