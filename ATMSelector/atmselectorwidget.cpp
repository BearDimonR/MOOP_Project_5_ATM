#include "atmselectorwidget.h"
#include "ui_atmselectorwidget.h"
#include <QHash>
#include <QString>
#include <iostream>
#include "atmselector.h"
#include "ATM/atm.h"
#include "mainwindow.h"

class ATMParams;

ATMSelectorWidget::ATMSelectorWidget(ATMSelector* out,QWidget *parent) :
    QWidget(parent),
    out_(out),
    ui_(new Ui::ATMSelectorWidget)

{
    ui_->setupUi(this);
    connect(out, SIGNAL(paramsChanged()), this, SLOT(onParamsUpdated()));
}


ATMSelectorWidget::~ATMSelectorWidget()
{
    delete ui_;
    delete mw;
}

void ATMSelectorWidget::onParamsUpdated()
{

    ui_->atmsList->clear();
    const QList<ATMParams> * newAtms(out_->params());
    for(int i = 0; i < newAtms->length(); ++i){
        const ATMParams* p = &(newAtms->at(i));
        QString item("АТМ: ");
        item.append(QString::number(p->atmId()));
        item.append(", Банк: ");
        item.append(p->bankName());
        hash.insert(item,i);
        ui_->atmsList->addItem(item);
        // ui_->atmsList->setCurrentRow(p->atmId());

    }

}

Ui::ATMSelectorWidget *ATMSelectorWidget::ui() const
{
    return ui_;
}

void ATMSelectorWidget::on_refreshButton_clicked()
{
    out_->refreshATMParams();
}

//що відбувається коли ми клікаємо на елемент зі списку
void ATMSelectorWidget::on_atmsList_itemActivated(QListWidgetItem *item)
{
    size_t id = hash.take(item->text());

    mw = new MainWindow(id);
    mw->show();


    cout<<"HUI"<<endl;

}
