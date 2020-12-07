#include "atm.h"
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
    connect(socket_, SIGNAL(replyOnQRSuccess()), this, SLOT(backQRSuccess()));
    connect(socket_, SIGNAL(replyOnValidatePin(const size_t)), this, SLOT(backValidatePin(const size_t)));
    connect(socket_, SIGNAL(replyOnSuccessPin()), this, SLOT(backPinSuccess()));
    connect(socket_, SIGNAL(replyOnChangePin()), this, SLOT(backChangePin()));
    connect(socket_, SIGNAL(replyOnSendToCard(const float, const float, const float,  const size_t)), this, SLOT(backSendToCard(const float, const float, const float,  const size_t)));
    connect(socket_, SIGNAL(replyOnCheckBal(const ATMCard&)), this, SLOT(backCheckBal(const ATMCard&)));
    connect(socket_, SIGNAL(replyOnTakeCash(const float, const float, const float,  const size_t)), this, SLOT(backTakeCash(const float, const float, const float,  const size_t)));

    emit atmStarted();
}

void ATM::backInsertCard()
{
    emit cardInserted();
}

void ATM::backFreeCard()
{
    if (card_ == Q_NULLPTR)
        delete card_;
    card_ = Q_NULLPTR;
    emit cardFree();
}

void ATM::backQRSuccess()
{
    emit qrSuccess();
}

void ATM::backPinSuccess()
{
    emit pinSuccess();
}

void ATM::backValidatePin(const size_t counter)
{
    emit pinValidated(counter);
}

void ATM::backChangePin()
{
    emit pinChanged();
}

void ATM::backSendToCard(const float bal, const float lim, const float aval,  const size_t inter)
{
    card_->updateBal(bal, lim, aval);
    emit cashSend(inter);
}

void ATM::backCheckBal(const ATMCard & card)
{
    if(card_ != Q_NULLPTR)
        delete card_;
    card_ = new ATMCard(card);
    emit balChecked();
}

void ATM::backTakeCash(const float bal, const float lim, const float aval,  const size_t inter)
{
    card_->updateBal(bal, lim, aval);
    emit cashTaken(inter);
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
    timer.start(10000);
    loop.exec();
    if(timer.isActive()) {
        connect(socket_, SIGNAL(replyOnStart(const ATMParams&)), this, SLOT(backOnStart(const ATMParams&)));
        connect(socket_, SIGNAL(replyOnError(const QString&)), this, SLOT(backError(const QString&)));
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


QString ATM::bankName() const
{
    return par_->bankName();
}

QPixmap ATM::qrcode() const
{
    return par_->getQRPixmap();
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
