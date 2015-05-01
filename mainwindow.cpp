#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QtSql>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->prod_curprod_buttons->button(QDialogButtonBox::Save)->setText(tr("Save"));
    this->ui->prod_curprod_buttons->button(QDialogButtonBox::Reset)->setText(tr("Reset"));
    this->ui->rec_currec_buttons->button(QDialogButtonBox::Save)->setText(tr("Save"));
    this->ui->rec_currec_buttons->button(QDialogButtonBox::Reset)->setText(tr("Reset"));
    this->ui->menu_curmenu_buttons->button(QDialogButtonBox::Save)->setText(tr("Save"));
    this->ui->menu_curmenu_buttons->button(QDialogButtonBox::Reset)->setText(tr("Reset"));

    currentProduct = -1;
    currentRecipe = -1;
    currentMenu = -1;

    buildProductTree();
    buildRecipeTree();
    buildMenuTree();

    initSettingsPanel();

    makeConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeConnections()
{
    // Tabs
    QObject::connect(this->ui->tabWidget,
                     SIGNAL(currentChanged(int)),
                     this,
                     SLOT(tabChanged(int)));

    // Trees
    QObject::connect(this->ui->prod_tree->selectionModel(),
                     SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                     this,
                     SLOT(treeItemSelected(const QModelIndex &, const QModelIndex &)));
    QObject::connect(this->ui->rec_tree->selectionModel(),
                     SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                     this,
                     SLOT(treeItemSelected(const QModelIndex &, const QModelIndex &)));
    QObject::connect(this->ui->menu_tree->selectionModel(),
                     SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                     this,
                     SLOT(treeItemSelected(const QModelIndex &, const QModelIndex &)));

    // Categories
    QObject::connect(this->ui->prod_cat,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SLOT(prodCatSelected(int)));
    QObject::connect(this->ui->rec_cat,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SLOT(recCatSelected(int)));
    QObject::connect(this->ui->menu_cat,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SLOT(menuCatSelected(int)));

    // Reset and Save Buttons
    QObject::connect(this->ui->prod_curprod_buttons,
                     SIGNAL(clicked(QAbstractButton*)),
                     this,
                     SLOT(generalProdButtonClicked(QAbstractButton*)));
    QObject::connect(this->ui->rec_currec_buttons,
                     SIGNAL(clicked(QAbstractButton*)),
                     this,
                     SLOT(generalRecButtonClicked(QAbstractButton*)));
    QObject::connect(this->ui->menu_curmenu_buttons,
                     SIGNAL(clicked(QAbstractButton*)),
                     this,
                     SLOT(generalMenuButtonClicked(QAbstractButton*)));


    // Delete item Buttons
    QObject::connect(this->ui->prod_delprod,
                     SIGNAL(clicked()),
                     this,
                     SLOT(deleteProduct()));
    QObject::connect(this->ui->rec_delrec,
                     SIGNAL(clicked()),
                     this,
                     SLOT(deleteRecipe()));
    QObject::connect(this->ui->menu_delmenu,
                     SIGNAL(clicked()),
                     this,
                     SLOT(deleteMenu()));

    // Insert item Buttons
    QObject::connect(this->ui->prod_newprod,
                     SIGNAL(clicked()),
                     this,
                     SLOT(insertNewProduct()));
    QObject::connect(this->ui->rec_newrec,
                     SIGNAL(clicked()),
                     this,
                     SLOT(insertNewRecipe()));
    QObject::connect(this->ui->menu_newmenu,
                     SIGNAL(clicked()),
                     this,
                     SLOT(insertNewMenu()));

    // Product
    QObject::connect(this->ui->prod_newprice,
                     SIGNAL(clicked()),
                     this,
                     SLOT(showNewPricePopup()));
    QObject::connect(this->ui->prod_editprice,
                     SIGNAL(clicked()),
                     this,
                     SLOT(showEditPricePopup()));
    QObject::connect(this->ui->prod_delprice,
                     SIGNAL(clicked()),
                     this,
                     SLOT(deletePrice()));
    QObject::connect(this->ui->prod_setcurrentprice,
                     SIGNAL(clicked()),
                     this,
                     SLOT(setCurrentPrice()));

    // Recipe
    QObject::connect(this->ui->rec_addprod,
                     SIGNAL(clicked()),
                     this,
                     SLOT(showAddProductPopup()));
    QObject::connect(this->ui->rec_editprod,
                     SIGNAL(clicked()),
                     this,
                     SLOT(showEditProductPopup()));
    QObject::connect(this->ui->rec_remprod,
                     SIGNAL(clicked()),
                     this,
                     SLOT(removeProduct()));

    // Settings Panel
    QObject::connect(this->ui->set_section,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SLOT(set_sectionSelected(int)));
    QObject::connect(this->ui->set_cat->selectionModel(),
                     SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                     this,
                     SLOT(set_catSelected(const QModelIndex &, const QModelIndex &)));
    QObject::connect(this->ui->set_cat_add,
                     SIGNAL(clicked()),
                     this,
                     SLOT(set_addCat()));
    QObject::connect(this->ui->set_cat_rem,
                     SIGNAL(clicked()),
                     this,
                     SLOT(set_deleteCat()));
    QObject::connect(this->ui->set_cat_ren,
                     SIGNAL(clicked()),
                     this,
                     SLOT(set_renameCat()));
    QObject::connect(this->ui->set_subcat_add,
                     SIGNAL(clicked()),
                     this,
                     SLOT(set_addSubCat()));
    QObject::connect(this->ui->set_subcat_rem,
                     SIGNAL(clicked()),
                     this,
                     SLOT(set_deleteSubCat()));
    QObject::connect(this->ui->set_subcat_ren,
                     SIGNAL(clicked()),
                     this,
                     SLOT(set_renameSubCat()));
    QObject::connect(this->ui->set_meas_add,
                     SIGNAL(clicked()),
                     this,
                     SLOT(set_addMeas()));
    QObject::connect(this->ui->set_meas_rem,
                     SIGNAL(clicked()),
                     this,
                     SLOT(set_deleteMeas()));
    QObject::connect(this->ui->set_meas_ren,
                     SIGNAL(clicked()),
                     this,
                     SLOT(set_renameMeas()));
    QObject::connect(this->ui->set_language,
                     SIGNAL(clicked()),
                     this,
                     SLOT(set_changeAppLanguage()));
}




