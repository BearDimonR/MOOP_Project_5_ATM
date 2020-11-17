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

    explicit ATMSelectorWidget(QWidget *parent = nullptr);

    ATMSelector* out_;
    Ui::ATMSelectorWidget *ui_;

    static ATMSelectorWidget& getInstance();
public:
    static void startSelector();
    static void hideSelector();

    QHash<QString,size_t> hash;
    Ui::ATMSelectorWidget *ui() const;

    virtual ~ATMSelectorWidget();

private slots:
    void showError(const QString&);
    void onParamsUpdated();
    void on_refreshButton_clicked();
    void on_atmsList_itemActivated(QListWidgetItem *item);
};

#endif // ATMSELECTORWIDGET_H
