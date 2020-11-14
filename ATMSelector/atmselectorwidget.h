#ifndef ATMSELECTORWIDGET_H
#define ATMSELECTORWIDGET_H

#include"ATM/Model/atmparams.h"
#include <QMainWindow>
#include <QListWidgetItem>
#include "mainwindow.h"

class ATMSelector;


namespace Ui {
class ATMSelectorWidget;
}

class ATMSelectorWidget : public QWidget
{
    Q_OBJECT
public:
    QHash<QString,size_t> hash;

    explicit ATMSelectorWidget(ATMSelector* out, QWidget *parent = nullptr);

    const QStringList takeParamsFromServer(ATMParams &params) const;

    ~ATMSelectorWidget();

    Ui::ATMSelectorWidget *ui() const;

private slots:


    void onParamsUpdated();

//signals:
//    void on_refreshButton_clicked();
    void on_refreshButton_clicked();

    void on_atmsList_itemActivated(QListWidgetItem *item);

private:

    ATMSelector* out_;
    Ui::ATMSelectorWidget *ui_;
    QMainWindow* mw;
};

#endif // ATMSELECTORWIDGET_H
