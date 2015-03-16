#ifndef RECIPEDIALOG_H
#define RECIPEDIALOG_H

#include <QDialog>

namespace Ui {
class RecipeDialog;
}

class RecipeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecipeDialog(QWidget *parent = 0);
    explicit RecipeDialog(int recIdentifier = -1, int prodIdentifier = -1, QWidget *parent = 0);
    ~RecipeDialog();

private:
    Ui::RecipeDialog *ui;
    int recId;
    int prodId;
    void fillCategoryList();

public slots:
    void addRecipe();
    void catSelected(int index);
    void subcatSelected(int index);
};

#endif // RECIPEDIALOG_H
