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
    connect(socket_, SIGNAL(replyOnStart(const ATMParams&)), this, SLOT(backOnStart(const ATMParams&)));
    connect(socket_, SIGNAL(replyOnInsertedCard()), this, SLOT(backInsertCard()));
    connect(socket_, SIGNAL(replyOnFreeCard()), this, SLOT(backFreeCard()));
    connect(socket_, SIGNAL(replyOnValidatePin(const size_t)), this, SLOT(backValidatePin(const size_t)));
    connect(socket_, SIGNAL(replyOnSuccessPin()), this, SLOT(backPinSuccess()));
    connect(socket_, SIGNAL(replyOnChangePin()), this, SLOT(backChangePin()));
    connect(socket_, SIGNAL(replyOnSendToCard(const ATMCard&)), this, SLOT(backSendToCard(const ATMCard&)));
    connect(socket_, SIGNAL(replyOnCheckBal(const ATMCard&)), this, SLOT(backCheckBal(const ATMCard&)));
    connect(socket_, SIGNAL(replyOnTakeCash(const ATMCard&, const long)), this, SLOT(backTakeCash(const ATMCard&, long)));

    connect(socket_, SIGNAL(replyOnError(const QString&)), this, SLOT(backError(const QString&)));

    emit atmStarted();
}

void ATM::backInsertCard()
{
    assert(card_ == Q_NULLPTR);
    emit cardInserted();
}

void ATM::backFreeCard()
{
    if (card_ == Q_NULLPTR)
        delete card_;
    card_ = Q_NULLPTR;
    emit cardFree();
}

void ATM::backPinSuccess()
{
    emit pinSuccess();
}

void ATM::backValidatePin(const size_t counter)
{
    assert(card_ == Q_NULLPTR);
    emit pinValidated(counter);
}

void ATM::backChangePin()
{
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
    delete card_;
    card_ = new ATMCard(card);
    emit balChecked();
}

void ATM::backTakeCash(const ATMCard & card, const long money)
{
    assert(card_ != Q_NULLPTR);
    delete card_;
    card_ = new ATMCard(card);
    long m = par_->cash();
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
    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;
    connect( socket_, &ATMSocket::replyOnConnected, &loop, &QEventLoop::quit );
    connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );
    timer.start(20000);
    loop.exec();
    if(timer.isActive()) {
        connect(socket_, SIGNAL(replyOnStart(const ATMParams&)), this, SLOT(backOnStart(const ATMParams&)));
        socket_->askStart(atm_id);
    }
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
    socket_->askInsertCard(number);
}

void ATM::freeCard()
{
    socket_->askFreeCard();
}

void ATM::validatePin(const size_t pin)
{
    socket_->askValidatePin(pin);
}

void ATM::changePin(const size_t pin)
{
    socket_->askChangePin(pin);
}

void ATM::sendToCard(const QString & number, const size_t sum)
{
    socket_->askSendToCard(number, sum);
}

void ATM::checkBal()
{
    socket_->askCheckBal();
}

void ATM::takeCash(const size_t sum)
{
    socket_->askTakeCash(sum);
}
