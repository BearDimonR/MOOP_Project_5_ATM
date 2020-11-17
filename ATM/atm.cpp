#include "atm.h"
#include <assert.h>
#include "ATM/Model/atmparams.h"
#include "ATM/Model/atmcard2.h"
#include "ATM/Socket/atmsocket.h"
#include "ATM/clienterror.h"
#include <QTimer>
#include <QEventLoop>

void ATM::backOnStart(const ATMParams & par)
{
    assert(par_ == Q_NULLPTR);
    par_ = new ATMParams(par);
    emit atmStarted();
}

void ATM::backInsertCard()
{
    assert(card_ == Q_NULLPTR);
    emit cardInserted();
}

void ATM::backFreeCard()
{
    assert(card_ != Q_NULLPTR);
    delete card_;
    card_ = Q_NULLPTR;
    emit cardFree();
}

void ATM::backPinSuccess(const ATMCard & card)
{
    assert(card_ == Q_NULLPTR);
    card_ = new ATMCard(card);
    emit pinSuccess();
}

void ATM::backValidatePin(const size_t counter)
{
    assert(card_ == Q_NULLPTR);
    emit pinValidated(counter);
}

void ATM::backChangePin()
{
    assert(card_ != Q_NULLPTR);
    emit pinChanged();
}

void ATM::backSendToCard(const ATMCard & card)
{
    assert(card_ != Q_NULLPTR);
    delete card_;
    card_ = new ATMCard(card);
    emit cashSend();
}

void ATM::backCheckBal(const ATMCard & card)
{
    assert(card_ != Q_NULLPTR);
    delete card_;
    card_ = new ATMCard(card);
    emit balChecked();
}

void ATM::backTakeCash(const ATMCard & card, long money)
{
    assert(card_ != Q_NULLPTR);
    delete card_;
    card_ = new ATMCard(card);
    long m = par_->cash();
    money= par_->cash();
    //par_->cash() = money;
    emit cashTaken(m - money);
}

void ATM::backError(const QString & error)
{
    emit errorOccured(error);
}

ATM::ATM(const size_t atm_id):
    socket_(new ATMSocket()),
    par_(Q_NULLPTR),
    card_(Q_NULLPTR)
{
    // deal with it
    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;
    connect( socket_, &ATMSocket::replyOnConnected, &loop, &QEventLoop::quit );
    connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );
    timer.start(5000);
    loop.exec();
    if(timer.isActive())
        socket_->askStart(atm_id);
    else
        qFatal(QString(ClientError("Starting error", ClientError::SERVER_REPLY_ERROR, "timeout")).toLatin1().constData());
}

ATM::~ATM()
{
    delete socket_;
    if(par_ != Q_NULLPTR)
        delete par_;
    if(card_ != Q_NULLPTR)
        delete card_;
}

ATMCard *ATM::card()
{
    return card_;
}

QString ATM::bankName()
{
    return par_->bankName();
}




void ATM::insertCard(const QString & number)
{
    assert(card_ == Q_NULLPTR);
    socket_->askInsertCard(number);
}

void ATM::freeCard()
{
    assert(card_ != Q_NULLPTR);
    socket_->askFreeCard();
}

void ATM::validatePin(const size_t pin)
{
    assert(card_ == Q_NULLPTR);
    socket_->askValidatePin(pin);
}

void ATM::changePin(const size_t pin)
{
    assert(card_ != Q_NULLPTR);
    socket_->askChangePin(pin);
}

void ATM::sendToCard(const QString & number, const size_t sum)
{
    assert(card_ != Q_NULLPTR);
    socket_->askSendToCard(number, sum);
}

void ATM::checkBal()
{
    assert(card_ != Q_NULLPTR);
    socket_->askCheckBal();
}

void ATM::takeCash(const size_t sum)
{
    assert(card_ != Q_NULLPTR);
    socket_->askTakeCash(sum);
}
