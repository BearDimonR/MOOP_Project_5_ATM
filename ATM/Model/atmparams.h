#ifndef ATMPARAMS_H
#define ATMPARAMS_H

#include <QJsonValue>
#include <QPixmap>

class ATMParams
{
public:
    static QString HOSTNAME;

private:
    size_t atm_id_;
    QString bank_name_;
    long cash_;
    size_t withdraw_int_;
    size_t transact_int_;
    QPixmap* qrcode_;

public:
    static ATMParams fromJson(const QJsonObject&);

    ATMParams(const size_t atm_id, const QString& bank_name,const long money,
              const size_t withdraw_int, const size_t transact_int_);

    ATMParams(const ATMParams&);
    ATMParams& operator=(const ATMParams&);

    ~ATMParams();

    operator QString() const;

    size_t atmId() const;
    const QString& bankName() const;
    long cash() const;
    size_t withdrawInterest() const;
    size_t transactInterest() const;

    void updateCash(const long);

    QPixmap getQRPixmap() const;

};

#endif // ATMPARAMS_H
