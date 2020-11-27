#ifndef ATMPARAMS_H
#define ATMPARAMS_H

#include <QJsonValue>
#include <QPixmap>

class ATMParams
{
public:
    enum Languages {UA = 1, EN = 2};

    static QString HOSTNAME;

private:
    size_t atm_id_;
    QString bank_name_;
    long cash_;
    Languages language_;
    size_t withdraw_int_;
    size_t transact_int_;
    QPixmap* qrcode_;

public:
    static ATMParams fromJson(const QJsonObject&);

    ATMParams(const size_t atm_id, const QString& bank_name,const long money,
              const size_t withdraw_int, const size_t transact_int_, const Languages lang = UA);

    ATMParams(const ATMParams&);
    ATMParams& operator=(const ATMParams&);

    ~ATMParams();

    operator QString() const;

    size_t atmId() const;
    const QString& bankName() const;
    Languages language() const;
    long cash() const;
    size_t withdrawInterest() const;
    size_t transactInterest() const;

    void updateCash(const long);

    QPixmap getQRPixmap();

};

#endif // ATMPARAMS_H
