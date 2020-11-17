#include "atmselectorwidget.h"
#include "ui_atmselectorwidget.h"
#include <QHash>
#include <QString>
#include <iostream>
#include "atmselector.h"
#include "ATM/atm.h"
#include "mainwindow.h"

class ATMParams;

ATMSelectorWidget::ATMSelectorWidget(QWidget *parent) :
    QWidget(parent),
    out_(new ATMSelector()),
    ui_(new Ui::ATMSelectorWidget)

{
    ui_->setupUi(this);
    connect(out_, SIGNAL(paramsChanged()), this, SLOT(onParamsUpdated()));
}

ATMSelectorWidget &ATMSelectorWidget::getInstance()
{
    static ATMSelectorWidget a;
    return a;
}

void ATMSelectorWidget::startSelector()
{
    ATMSelectorWidget::getInstance().show();
}

void ATMSelectorWidget::hideSelector()
{
    ATMSelectorWidget::getInstance().hide();
}


ATMSelectorWidget::~ATMSelectorWidget()
{
    delete ui_;
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
    // типа не всегда работает почему-то
    //size_t id = hash.take(item->text());
    MainWindow::startMainWindow(1);
}
