#ifndef ATMSELECTORWIDGET_H
#define ATMSELECTORWIDGET_H

#include"ATM/Model/atmparams.h"
#include <QMainWindow>
#include <QListWidgetItem>

class ATM;
class ATMSelector;


namespace Ui {
class ATMSelectorWidget;
}

class ATMSelectorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ATMSelectorWidget(ATMSelector* out, QWidget *parent = nullptr);

    const QStringList takeParamsFromServer(ATMParams &params) const;

    ~ATMSelectorWidget();

    Ui::ATMSelectorWidget *ui() const;

private slots:
    void onParamsUpdated();
    void on_refreshButton_clicked();
    void on_atmsList_itemPressed(QListWidgetItem *item);

signals:
    void atm_selected(const size_t);

private:
    ATMSelector* out_;
    Ui::ATMSelectorWidget *ui_;
};

#endif // ATMSELECTORWIDGET_H
