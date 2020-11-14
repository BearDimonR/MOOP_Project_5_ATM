#include "atmselector.h"
#include "atmselectorwidget.h"
#include "ATM/Socket/atmselectorsocket.h"
#include "ATM/Model/atmparams.h"
#include <QJsonArray>

void ATMSelector::updateParams(const QList<ATMParams> & l)
{
    QList<ATMParams>* p(params_);
    params_ = new QList<ATMParams>(l);
    delete p;
    emit paramsChanged();
}

ATMSelector::ATMSelector(QObject *parent):
    QObject(parent),
    params_(new QList<ATMParams>),
    socket_(new ATMSelectorSocket())
{
    connect(socket_, &ATMSelectorSocket::receivedATMParams,
            this, &ATMSelector::updateParams);
}

ATMSelector::~ATMSelector(){
    socket_->disconnect();
    delete socket_;
    delete params_;
}

const QList<ATMParams>* ATMSelector::params() const
{
    return params_;
}

void ATMSelector::refreshATMParams()
{
    socket_->askForATMParams();
}

const QList<ATMParams> *ATMSelector::getParams() const
{
    return params_;
}
