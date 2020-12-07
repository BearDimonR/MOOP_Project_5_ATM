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
    QPixmap* qrcode_;

public:
    static ATMParams fromJson(const QJsonObject&);

    ATMParams(const size_t atm_id, const QString& bank_name,const long money);

    ATMParams(const ATMParams&);
    ATMParams& operator=(const ATMParams&);

    ~ATMParams();

    operator QString() const;

    size_t atmId() const;
    const QString& bankName() const;
    long cash() const;

    void updateCash(const long);

    QPixmap getQRPixmap() const;

};

#endif // ATMPARAMS_H
