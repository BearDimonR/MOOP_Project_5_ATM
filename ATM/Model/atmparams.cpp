#include "atmparams.h"
#include "ATM/clienterror.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <bqrcodegen.h>
#include <bqrimagefactory.h>
#include <bqrdecoratedcolorfactory.h>
#include <bqrdefaultimagefactory.h>
#include "Utility/utilities.h"
#include <bqrdecoratedcolorfactory.h>

using namespace BQREncode;

QString ATMParams::HOSTNAME = Utility::getInstance().getString("QR_HOSTNAME");

ATMParams ATMParams::fromJson(const QJsonObject & obj)
{
    QJsonValue atm_id(obj["atm_id"]);
    QJsonValue bank_name(obj["bank_name"]);
    QJsonValue cash(obj["cash"]);
    QJsonValue withdraw(obj["withdraw_interest"]);
    QJsonValue transact(obj["transact_interest"]);
    if(atm_id.isNull() || atm_id.isUndefined() || !atm_id.isDouble()
            || bank_name.isNull() || bank_name.isUndefined() || !bank_name.isString()
            || cash.isNull() || cash.isUndefined() || !cash.isDouble()
            || withdraw.isNull() || withdraw.isUndefined() || !withdraw.isDouble()
            || transact.isNull() || transact.isUndefined() || !transact.isDouble())
        qFatal(QString(ClientError("ATMParams json error", ClientError::PARSING_ERROR, QJsonDocument(obj).toBinaryData())).toLatin1().constData());
    return ATMParams(atm_id.toVariant().toULongLong(), bank_name.toVariant().toString(),
                 cash.toVariant().toULongLong(), withdraw.toVariant().toUInt(), withdraw.toVariant().toUInt(), ATMParams::Languages::UA);
}


ATMParams::ATMParams(const size_t atm_id, const QString &bank_name, const long cash,
                     const size_t withdraw, const size_t transact, const Languages lang):
    atm_id_(atm_id),
    bank_name_(bank_name),
    cash_(cash),
    language_(lang),
    withdraw_int_(withdraw),
    transact_int_(transact),
    qrcode_(Q_NULLPTR)
{
}

ATMParams::ATMParams(const ATMParams & p):
    atm_id_(p.atm_id_),
    bank_name_(p.bank_name_),
    cash_(p.cash_),
    language_(p.language_),
    withdraw_int_(p.withdraw_int_),
    transact_int_(p.transact_int_),
    qrcode_(p.qrcode_)
{}

ATMParams &ATMParams::operator=(const ATMParams & that)
{
    if(this == &that)
        return *this;
    atm_id_ = that.atm_id_;
    bank_name_ = that.bank_name_;
    cash_ = that.cash_;
    language_ = that.language_;
    withdraw_int_ = that.withdraw_int_;
    transact_int_ = that.transact_int_;
    if (qrcode_ != Q_NULLPTR)
        delete qrcode_;
    qrcode_ = that.qrcode_;
    return *this;
}

ATMParams::~ATMParams()
{
    if (qrcode_ != Q_NULLPTR)
        delete qrcode_;
}

ATMParams::operator QString() const
{
    return "АТМ: " + QString::number(atm_id_) + ", Банк: " + bank_name_;
}

size_t ATMParams::atmId() const
{
    return atm_id_;
}

const QString &ATMParams::bankName() const
{
    return bank_name_;
}


ATMParams::Languages ATMParams::language() const
{
    return language_;
}

long ATMParams::cash() const
{
    return cash_;
}

size_t ATMParams::withdrawInterest() const
{
    return withdraw_int_;
}

size_t ATMParams::transactInterest() const
{
    return transact_int_;
}

void ATMParams::updateCash(const long cash)
{
    cash_ = cash;
}



QPixmap ATMParams::getQRPixmap()
{
    if (qrcode_ != Q_NULLPTR)
           return *qrcode_;

    BQRCodeGen generator;
    BQRCode *codeQR=generator.encode(HOSTNAME + QString::number(atm_id_));
    BQRColorFactory *factory=new BQRColorFactory;

    //factory->setColor(QColor(255,0,0));
//    factory->setBackgroundDecorationSize(QSize(400,400));
//    factory->setPathDecoration(":/imgs/img/kisspng-check-mark-bottle-material-green-tick-5ad25467123860.2792666715237336070746.png");
//    factory->setSize(QSize(400,400));
    QImage *qrCodeImage=factory->buildImageFromCode(codeQR);
    QPixmap res(QPixmap::fromImage(*qrCodeImage));
    delete codeQR;
    delete factory;
    delete qrCodeImage;
    return res;


}

