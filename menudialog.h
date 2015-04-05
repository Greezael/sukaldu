#ifndef MENUDIALOG_H
#define MENUDIALOG_H

#include <QDialog>

namespace Ui {
class MenuDialog;
}

class MenuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MenuDialog(QWidget *parent = 0);
    explicit MenuDialog(int menuIdentifier, int roleIdentifier, QWidget *parent = 0);
    ~MenuDialog();

private:
    int menuId;
    int roleId;
    Ui::MenuDialog *ui;
    void fillCategoryList();

public slots:
    void addRecipe();
    void catSelected(int index);
    void subcatSelected(int index);
};

#endif // MENUDIALOG_H
