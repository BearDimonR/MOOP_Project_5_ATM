#include "atmsocket.h"
#include "Utility/utilities.h"
#include "ATM/Model/atmparams.h"
#include "ATM/Model/atmcard2.h"
#include "ATM/clienterror.h"
#include <QJsonArray>
#include <QVariant>
#include <QJsonValue>

QList<QString> ATMSocket::EVENT_STRINGS = Utility::getInstance().getStringArr("ATMSocket_events");

void ATMSocket::doOnConnected()
{
    AppSocket::doOnConnected();
    emit replyOnConnected();
}

void ATMSocket::doOnTextMessageReceived(const QJsonObject & in)
{
    //TO REMOVE
    qDebug() << "Received json (ATMSocket):\n" << QJsonDocument(in).toJson() << "\n\n";
    //TO REMOVE

    QJsonValue ev(in["event"]);
    QJsonValue pl(in["payload"]);
    if(ev.isNull() || ev.isUndefined() || !ev.isString())
        qFatal("%s", QString(ClientError("ATMSocket on receive json error",
                       ClientError::PARSING_ERROR, QJsonDocument(in).toJson())).toLatin1().constData());
    QJsonObject plobj(pl.toObject());
    QJsonValue er(plobj["error"]);
    QJsonValue ct(plobj["content"]);
    if(!er.isNull() && !er.isUndefined() && er.isString())
    {
        emit replyOnError(er.toString());
        return;
    }
    QJsonObject obj = ct.toObject();
    switch (EVENT_STRINGS.indexOf(ev.toString())) {
        case EVENTS::START_ATM:
                emit replyOnStart(ATMParams::fromJson(obj));
                break;
        case EVENTS::INSERT_CARD:
                emit replyOnInsertedCard();
                break;
        case EVENTS::SUCCESS_PIN:
                emit replyOnSuccessPin();
                break;
        case EVENTS::FREE_CARD:
                emit replyOnFreeCard();
                break;
        case EVENTS::CHANGE_PIN:
                emit replyOnChangePin();
                break;
        case EVENTS::SEND_TO_CARD:
                emit replyOnSendToCard(ATMCard::fromJson(obj));
                break;
        case EVENTS::CHECK_BAL:
                emit replyOnCheckBal(ATMCard::fromJson(obj));
                break;
        case EVENTS::CHECK_PIN:
                {
                    QJsonValue counter(obj["counter"]);
                    if(counter.isNull() || counter.isUndefined() || !counter.isDouble())
                                        qFatal("%s", QString(ClientError("ATMSocket on receive json check_pin error",
                                                       ClientError::PARSING_ERROR, QJsonDocument(in).toJson())).toLatin1().constData());
                    emit replyOnValidatePin(counter.toVariant().toULongLong());
                    break;
                }
        case EVENTS::TAKE_FROM_CARD:
                {
                    QJsonValue cash(obj["atm_cash"]);
                    if(cash.isNull() || cash.isUndefined() || !cash.isDouble())
                                        qFatal("%s", QString(ClientError("ATMSocket on receive json take_from_card error",
                                                       ClientError::PARSING_ERROR, QJsonDocument(in).toJson())).toLatin1().constData());
                    emit replyOnTakeCash(ATMCard::fromJson(obj), cash.toVariant().toLongLong());
                    break;
                }
        default:
                qFatal("%s", QString(ClientError("ATMSocket on receive json error",
                               ClientError::UNDEFINED_EVENT, QJsonDocument(in).toJson())).toLatin1().constData());
    }
}

ATMSocket::ATMSocket(QObject *parent):
    AppSocket(parent)
{}

ATMSocket::~ATMSocket()
{}

// a lot of duplicates

void ATMSocket::askStart(const size_t atm_id)
{
    QJsonObject obj;
    obj.insert("number", QJsonValue(static_cast<qint64>(atm_id)));
    sendMessage(EVENT_STRINGS.at(EVENTS::START_ATM), QJsonDocument(obj).toJson());
}

void ATMSocket::askInsertCard(const QString & number)
{
    sendMessage(EVENT_STRINGS.at(EVENTS::INSERT_CARD), number);
}

void ATMSocket::askFreeCard()
{
    sendMessage(EVENT_STRINGS.at(EVENTS::FREE_CARD), "");
}

void ATMSocket::askValidatePin(const size_t pin)
{
    sendMessage(EVENT_STRINGS.at(EVENTS::CHECK_PIN), QString::number(pin));
}

void ATMSocket::askChangePin(const size_t pin)
{
    sendMessage(EVENT_STRINGS.at(EVENTS::CHANGE_PIN), QString::number(pin));
}

void ATMSocket::askSendToCard(const QString & number, const size_t sum)
{
    QJsonObject obj;
    obj.insert("number", number);
    obj.insert("sum", QJsonValue(static_cast<qint64>(sum)));
    sendMessage(EVENT_STRINGS.at(EVENTS::SEND_TO_CARD), QJsonDocument(obj).toJson());
}

void ATMSocket::askCheckBal()
{
    sendMessage(EVENT_STRINGS.at(EVENTS::CHECK_BAL), "");
}

void ATMSocket::askTakeCash(const size_t sum)
{
    sendMessage(EVENT_STRINGS.at(EVENTS::TAKE_FROM_CARD), QString::number(sum));
}
