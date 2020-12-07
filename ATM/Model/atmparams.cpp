#include "atmparams.h"
#include "ATM/clienterror.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <bqrcodegen.h>
#include <bqrimagefactory.h>
#include <bqrdefaultimagefactory.h>
#include "Utility/utilities.h"

using namespace BQREncode;

QString ATMParams::HOSTNAME = Utility::getInstance().getString("QR_HOSTNAME");

ATMParams ATMParams::fromJson(const QJsonObject & obj)
{
    QJsonValue atm_id(obj["atm_id"]);
    QJsonValue bank_name(obj["bank_name"]);
    QJsonValue cash(obj["cash"]);
    if(atm_id.isNull() || atm_id.isUndefined() || !atm_id.isDouble()
            || bank_name.isNull() || bank_name.isUndefined() || !bank_name.isString()
            || cash.isNull() || cash.isUndefined() || !cash.isDouble())
        qFatal(QString(ClientError("ATMParams json error", ClientError::PARSING_ERROR, QJsonDocument(obj).toBinaryData())).toLatin1().constData());
    return ATMParams(atm_id.toVariant().toULongLong(), bank_name.toVariant().toString(),
                 cash.toVariant().toULongLong());
}


ATMParams::ATMParams(const size_t atm_id, const QString &bank_name, const long cash):
    atm_id_(atm_id),
    bank_name_(bank_name),
    cash_(cash),
    qrcode_(Q_NULLPTR)
{
}

ATMParams::ATMParams(const ATMParams & p):
    atm_id_(p.atm_id_),
    bank_name_(p.bank_name_),
    cash_(p.cash_),
    qrcode_(p.qrcode_)
{}

ATMParams &ATMParams::operator=(const ATMParams & that)
{
    if(this == &that)
        return *this;
    atm_id_ = that.atm_id_;
    bank_name_ = that.bank_name_;
    cash_ = that.cash_;
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

long ATMParams::cash() const
{
    return cash_;
}

void ATMParams::updateCash(const long cash)
{
    cash_ = cash;
}



QPixmap ATMParams::getQRPixmap() const
{
    if (qrcode_ != Q_NULLPTR)
           return *qrcode_;
    BQRCodeGen generator;
    BQRCode *codeQR=generator.encode(HOSTNAME + QString::number(atm_id_));
    BQRImageFactory *factory= new BQRDefaultImageFactory();
    QImage *qrCodeImage=factory->buildImageFromCode(codeQR);
    for(int y = 0; y < qrCodeImage->height(); y++)
    {
      for(int x= 0; x < qrCodeImage->width(); x++)
      {
          if(qrCodeImage->pixelColor(x,y) == Qt::white)
                qrCodeImage->setPixelColor(x,y,QColor(255, 234, 189));
      }
    }
    QPixmap res(QPixmap::fromImage(*qrCodeImage));
    delete codeQR;
    delete factory;
    delete qrCodeImage;
    return res;


}

