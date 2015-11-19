#ifndef COMPARISONDIALOG_H
#define COMPARISONDIALOG_H

#include <QDialog>
#include "ui_comparisondialog.h"

namespace Ui {
class ComparisonDialog;
}

class ComparisonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ComparisonDialog(QWidget *parent = 0);
    ~ComparisonDialog();
    Ui::ComparisonDialog *ui;
};

#endif // COMPARISONDIALOG_H
