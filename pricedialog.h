#ifndef PRICEDIALOG_H
#define PRICEDIALOG_H

#include <QDialog>

namespace Ui {
class PriceDialog;
}

class PriceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PriceDialog(QWidget *parent = 0);
    explicit PriceDialog(int prodIdentifier = -1, int priceIdentifier = -1, QWidget *parent = 0);
    ~PriceDialog();

    inline PriceDialog& product(int prodId)
    { this->prodId = prodId; return *this; }

private:
    Ui::PriceDialog *ui;
    int prodId;
    int priceId;


public slots:
    void addPrice();
};

#endif // PRICEDIALOG_H
