#include "atmselectorwidget.h"
#include "ui_atmselectorwidget.h"
#include "atmselector.h"
#include "ATM/atm.h"
#include "mainwindow.h"
#include <QHash>
#include <QString>
#include <QMessageBox>
#include <QVariant>

ATMSelectorWidget::ATMSelectorWidget(QWidget *parent) :
    QWidget(parent),
    out_(new ATMSelector()),
    ui_(new Ui::ATMSelectorWidget)

{
    ui_->setupUi(this);
    connect(out_, SIGNAL(paramsChanged()), this, SLOT(onParamsUpdated()));
    connect(out_, SIGNAL(errorOccured(const QString&)), this, SLOT(showError(const QString&)));
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

void ATMSelectorWidget::showError(const QString & er)
{
    QMessageBox::critical(this, tr("ATMSelectorWidget"), er);
}

void ATMSelectorWidget::onParamsUpdated()
{

    ui_->atmsList->clear();
    const QList<ATMParams> * newAtms(out_->params());
    for(int i = 0; i < newAtms->length(); ++i){
        ui_->atmsList->addItem(QString(newAtms->at(i)));
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

void ATMSelectorWidget::on_atmsList_itemActivated(QListWidgetItem *item)
{
    MainWindow::startMainWindow(out_->params()->at(ui_->atmsList->currentRow()).atmId());
}
