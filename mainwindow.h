#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

enum SK_CustomRole {
    SK_IdRole = Qt::UserRole + 1,
    SK_TypeRole
};

enum SK_ObjectType {
    SK_Wrapper,
    SK_Product,
    SK_Recipe,
    SK_Menu

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

    void buildTree();

public slots:
    void treeItemSelected(const QModelIndex &current, const QModelIndex &previous);
};

#endif // MAINWINDOW_H
