#ifndef ATMSELECTORWIDGET_H
#define ATMSELECTORWIDGET_H

#include"ATM/Model/atmparams.h"
#include <QMainWindow>
#include <QListWidgetItem>
#include "mainwindow.h"

class ATM;
class ATMSelector;


namespace Ui {
class ATMSelectorWidget;
}

class ATMSelectorWidget : public QWidget
{

    Q_OBJECT

private:

    ATMSelectorWidget(const ATMSelectorWidget&) = delete;
    ATMSelectorWidget(const ATMSelectorWidget&&) = delete;
    ATMSelectorWidget& operator=(const ATMSelectorWidget&) = delete;
    ATMSelectorWidget& operator=(ATMSelectorWidget&&) = delete;

    ATMSelector* out_;
    Ui::ATMSelectorWidget *ui_;

public:
    QHash<QString,size_t> hash;
    Ui::ATMSelectorWidget *ui() const;

    explicit ATMSelectorWidget(ATMSelector* out, QWidget *parent = nullptr);
    ~ATMSelectorWidget();

private slots:
    void onParamsUpdated();
    void on_refreshButton_clicked();
    void on_atmsList_itemActivated(QListWidgetItem *item);

signals:
    void atm_selected(const size_t);
};

#endif // ATMSELECTORWIDGET_H
