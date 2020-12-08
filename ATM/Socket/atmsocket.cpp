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

void ATMSocket::doOnDisconnected()
{
    AppSocket::doOnDisconnected();
    qFatal("%s", QString(ClientError("ATMSocket on receive json error",
                                     ClientError::CONNECTION_ERROR)).toLatin1().constData());
}

void ATMSocket::doOnTextMessageReceived(const QJsonObject & in)
{
    qDebug() << "Received json (ATMSocket):\n" << QJsonDocument(in).toJson() << "\n\n";

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
    QJsonValue aval2(ct.toObject()["credit_avaliable"]);
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
        case EVENTS::QR_SUCCESS:
                emit replyOnQRSuccess();
                break;
        case EVENTS::CHANGE_PIN:
                emit replyOnChangePin();
                break;
        case EVENTS::CHECK_BAL:
                emit replyOnCheckBal(ATMCard::fromJson(obj));
                break;
        case EVENTS::SEND_TO_CARD:
                {
                    QJsonValue bal(obj["balance"]);
                    QJsonValue lim(obj["cred_limit"]);
                    QJsonValue aval(obj["credit_available"]);
                    QJsonValue inter(obj["interest"]);
                    if(bal.isNull() || bal.isUndefined() || !bal.isDouble()
                            || lim.isNull() || lim.isUndefined() || !lim.isDouble()
                            || aval.isNull() || aval.isUndefined() || !aval.isDouble()
                            || inter.isNull() || inter.isUndefined() || !inter.isDouble())
                                        qFatal("%s", QString(ClientError("ATMSocket on receive json send_to_card error",
                                                       ClientError::PARSING_ERROR, QJsonDocument(in).toJson())).toLatin1().constData());
                    emit replyOnSendToCard(bal.toVariant().toDouble(),
                                           lim.toVariant().toDouble(),
                                           aval.toVariant().toDouble(),
                                           inter.toVariant().toUInt());
                }
                break;
        case EVENTS::TAKE_FROM_CARD:
                {
                    QJsonValue bal(obj["balance"]);
                    QJsonValue lim(obj["cred_limit"]);
                    QJsonValue aval(obj["credit_available"]);
                    QJsonValue inter(obj["interest"]);
                    if(bal.isNull() || bal.isUndefined() || !bal.isDouble()
                            || lim.isNull() || lim.isUndefined() || !lim.isDouble()
                            || aval.isNull() || aval.isUndefined() || !aval.isDouble()
                            || inter.isNull() || inter.isUndefined() || !inter.isDouble())
                                        qFatal("%s", QString(ClientError("ATMSocket on receive json take_cash error",
                                                       ClientError::PARSING_ERROR, QJsonDocument(in).toJson())).toLatin1().constData());
                    emit replyOnTakeCash(bal.toVariant().toDouble(),
                                           lim.toVariant().toDouble(),
                                           aval.toVariant().toDouble(),
                                            inter.toVariant().toUInt());
                }
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

void ATMSocket::askValidatePin(const QString& pin)
{
    sendMessage(EVENT_STRINGS.at(EVENTS::CHECK_PIN), pin);
}

void ATMSocket::askChangePin(const QString& pin)
{
    sendMessage(EVENT_STRINGS.at(EVENTS::CHANGE_PIN), pin);
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
