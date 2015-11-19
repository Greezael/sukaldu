#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <QRect>

#include "comparisondialog.h"

enum SK_CustomRole {
    SK_IdRole = Qt::UserRole + 1,
    SK_TypeRole
};

enum SK_ObjectType {
    SK_Wrapper,
    SK_Product,
    SK_Recipe,
    SK_Menu,
    SK_Category,
    SK_Subcategory
};

enum SK_Section {
    SK_S_NONE,
    SK_S_PROD,
    SK_S_REC,
    SK_S_MENU
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void makeConnections();
    int currentCatId, currentSubCatId;

// Product related
private:
    void buildProductTree();
    void fillProductCategoryLists(int catId, int subCatId);
    void fillProductMeasurementList(int measId);
    void productSelected(int id);
    void resetProductData();
    void saveProductData();

public:
    void updatePriceList();

public slots:
    void prodCatSelected(int index);
    void showNewPricePopup();
    void showEditPricePopup();
    void setCurrentPrice();
    void deletePrice();
    void generalProdButtonClicked(QAbstractButton * button);
    void insertNewProduct();
    void deleteProduct();

// Recipe related
private:
    void buildRecipeTree();
    void fillRecipeCategoryLists(int catId, int subCatId);
    void recipeSelected(int id);
    void resetRecipeData();
    void saveRecipeData();
    void updateRecipePrice();

public:
    void updateIngredientsList();

public slots:
    void recCatSelected(int index);
    void generalRecButtonClicked(QAbstractButton * button);
    void insertNewRecipe();
    void deleteRecipe();
    void showAddProductPopup();
    void showEditProductPopup();
    void removeProduct();

// Menu related
private:
    void buildMenuTree();
    void fillMenuCategoryLists(int catId, int subCatId);
    void menuSelected(int id);
    void resetMenuData();
    void saveMenuData();

    void fillMenuOptions();
    void cleanMenuOptions();
    std::vector<QVariant> menuOptions;
    int firstMenuOptionRow;
    void showMenuOption(QVariant roleid);

public:
    void updateMenuPrice();
    void reloadMenuOptions();

public slots:
    void menuCatSelected(int index);
    void generalMenuButtonClicked(QAbstractButton * button);
    void showAddRecipePopup(int row);
    void removeRecipes(int row);
    void addOption();
    void removeOption(int row);
    void renameOption(int row);
    void insertNewMenu();
    void deleteMenu();

// Generic code
private:
    void buildTree(SK_Section section);
    void fillCategoryLists(int catId, int subCatId, SK_Section section);
    void deleteItems(SK_Section section);
    QString currencyFormatter(QVariant value);

public slots:
    void treeItemSelected(const QModelIndex &current, const QModelIndex &previous);
    void catSelected(int index, SK_Section section);
    void tabChanged(int index);

// Statistics related
public:
    ComparisonDialog * compDialog;
    QRect compDialogGeometry;
private:
    void sta_loadTableRecipe();
    void sta_loadTableMenu();
public slots:
    void sta_loadTables();
    void toggleComparisonDialog();


// Settings related
private:
    SK_Section currentSection;
    int currentSectionIndex;
    int currentCategory;
    void initSettingsPanel();
    void rebuildTrees();
    void resetCategoriesInfo();
    void resetSubCategoriesInfo();
    void resetMeasInfo();
    void resetProvInfo();
    void resetCurrency();
    void resetAdvancedOpts();

public slots:
    void set_sectionSelected(int index);
    void set_catSelected(const QModelIndex & current, const QModelIndex & previous);
    void set_catSelectedById(int id);
    void set_addCat();
    void set_deleteCat();
    void set_renameCat();
    void set_addSubCat();
    void set_renameSubCat();
    void set_deleteSubCat();
    void set_addMeas();
    void set_renameMeas();
    void set_deleteMeas();
    void set_addProv();
    void set_renameProv();
    void set_deleteProv();
    void set_changeAppLanguage();
    void set_currencyChanged(int id);
    void set_advancedOptsChanged(bool state);

// State
private:
    int currentProduct;
    int currentRecipe;
    int currentMenu;
};

#endif // MAINWINDOW_H
