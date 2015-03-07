#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>

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

// Product related
private:
    void buildProductTree();
    void fillProductCategoryLists(int catId, int subCatId);
    void fillProductMeasurementList(int measId);
    void productSelected(int id);
    void resetProductData();
    void saveProductData();
    int currentCatId, currentSubCatId;

public slots:
    void prodCatSelected(int index);
    void showNewPricePopup();
    void setCurrentPrice();
    void generalProdButtonClicked(QAbstractButton * button);
    void insertNewProduct();
    void deleteProduct();
    void updatePriceList();

// Recipe related
private:
    void buildRecipeTree();
    void recipeSelected(int id);

// Menu related
private:
    void buildMenuTree();


// Generic code
private:
    void buildTree(SK_Section section);

public slots:
    void treeItemSelected(const QModelIndex &current, const QModelIndex &previous);

// State
private:
    int currentProduct;
    int currentRecipe;
    int currentMenu;
};

#endif // MAINWINDOW_H
