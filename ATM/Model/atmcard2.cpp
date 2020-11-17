#include "atmcard2.h"
#include "ATM/clienterror.h"
#include <QJsonDocument>
#include <QVariant>

ATMCard ATMCard::fromJson(const QJsonObject & obj)
{
    QJsonValue bal(obj["balance"]);
    QJsonValue cre_a(obj["credit_available"]);
    QJsonValue cre_l(obj["cred_limit"]);
    if(bal.isNull() || bal.isUndefined() || !bal.isDouble()
            || cre_a.isNull() || cre_a.isUndefined() || !cre_a.isDouble()
            || cre_l.isNull() || cre_l.isUndefined() || !cre_l.isDouble())
        qFatal(QString(ClientError("ATMCard json error", ClientError::PARSING_ERROR, QJsonDocument(obj).toBinaryData())).toLatin1().constData());
    return ATMCard(bal.toVariant().toLongLong(),
                         cre_a.toVariant().toLongLong(),
                          cre_l.toVariant().toLongLong());

}

ATMCard::ATMCard(const long bal, const long credA, const long credL):
    bal_(bal),
    creditAval_(credA),
    creditLim_(credL)
{}

long ATMCard::bal() const
{
    return bal_;
}

long ATMCard::creditAval() const
{
    return creditAval_;
}

long ATMCard::creditLim() const
{
    return creditLim_;
}


ATMCard::~ATMCard()
{}

ATMCard::ATMCard(const ATMCard & that)
{
    bal_ = that.bal_;
    creditLim_ = that.creditLim_;
    creditAval_ = that.creditAval_;
}

ATMCard &ATMCard::operator=(const ATMCard & that)
{
    if(this == &that)
        return *this;
    bal_ = that.bal_;
    creditLim_ = that.creditLim_;
    creditAval_ = that.creditAval_;
    return *this;
}
