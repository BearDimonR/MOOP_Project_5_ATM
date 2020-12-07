#include "atmcard2.h"
#include "ATM/clienterror.h"
#include <QJsonDocument>
#include <QVariant>

ATMCard ATMCard::fromJson(const QJsonObject & obj)
{
    QJsonValue bal(obj["balance"]);
    QJsonValue cre_a(obj["credit_available"]);
    QJsonValue cre_l(obj["cred_limit"]);
    QJsonValue wth(obj["withdraw_interest"]);
    QJsonValue tran(obj["transact_interest"]);
    if(bal.isNull() || bal.isUndefined() || !bal.isDouble()
            || cre_a.isNull() || cre_a.isUndefined() || !cre_a.isDouble()
            || cre_l.isNull() || cre_l.isUndefined() || !cre_l.isDouble()
            || wth.isNull() || wth.isUndefined() || !wth.isDouble()
            || tran.isNull() || tran.isUndefined() || !tran.isDouble())
        qFatal(QString(ClientError("ATMCard json error", ClientError::PARSING_ERROR, QJsonDocument(obj).toBinaryData())).toLatin1().constData());
    return ATMCard(bal.toVariant().toDouble(),
                         cre_a.toVariant().toFloat(),
                          cre_l.toVariant().toFloat(),
                            wth.toVariant().toUInt(),
                                tran.toVariant().toUInt());

}

void ATMCard::updateBal(const float bal, const float lim, const float aval)
{
    bal_ = bal;
    creditLim_ = lim;
    creditAval_ = aval;
}

ATMCard::ATMCard(const float bal, const float credA, const float credL, const size_t with, const size_t trans):
    bal_(bal),
    creditAval_(credA),
    creditLim_(credL),
    with_int_(with),
    trans_int_(trans)
{}

float ATMCard::bal() const
{
    return bal_;
}

float ATMCard::creditAval() const
{
    return creditAval_;
}

float ATMCard::creditLim() const
{
    return creditLim_;
}

size_t ATMCard::withdraw() const
{
    return with_int_;
}

size_t ATMCard::transact() const
{
    return trans_int_;
}


ATMCard::~ATMCard()
{}

ATMCard::ATMCard(const ATMCard & that)
{
    bal_ = that.bal_;
    creditLim_ = that.creditLim_;
    creditAval_ = that.creditAval_;
    with_int_ = that.with_int_;
    trans_int_ = that.trans_int_;
}

ATMCard &ATMCard::operator=(const ATMCard & that)
{
    if(this == &that)
        return *this;
    bal_ = that.bal_;
    creditLim_ = that.creditLim_;
    creditAval_ = that.creditAval_;
    with_int_ = that.with_int_;
    trans_int_ = that.trans_int_;
    return *this;
}
