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

// Product related
private:
    void makeProductConnections();
    void buildProductTree();
    void fillProductCategoryLists(int catId, int subCatId);
    void fillProductMeasurementList(int measId);
    void updatePriceList();
    void productSelected(int id);
    void resetProductData();

public slots:
    void prodTreeItemSelected(const QModelIndex &current, const QModelIndex &previous);
    void showNewPricePopup();
    void generalProdButtonClicked(QAbstractButton * button);


// State
private:
    int currentProduct;
    int currentRecipe;
    int currentMenu;
};

#endif // MAINWINDOW_H
