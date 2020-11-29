#ifndef ATM_H
#define ATM_H

#include <QObject>

#include "ATM/Model/atmcard2.h"
#include "ATM/Model/atmparams.h"
#include "ATM/Socket/atmsocket.h"

class ATM : public QObject
{
    Q_OBJECT

protected:
    ATMSocket* socket_;
    ATMParams* par_;
    ATMCard* card_;

    ATM(const ATM&) = delete;
    ATM(ATM&&) = delete;
    ATM& operator=(const ATM&) = delete;
    ATM& operator=(ATM&&) = delete;

protected slots:
    void backOnStart(const ATMParams&);
    void backInsertCard();
    void backFreeCard();
    void backQRSuccess();
    void backValidatePin(const size_t);
    void backPinSuccess();
    void backChangePin();
    void backSendToCard(const ATMCard&);
    void backCheckBal(const ATMCard&);
    void backTakeCash(const ATMCard&, long);

    void backError(const QString&);

public:
    explicit ATM(const size_t);
    virtual ~ATM();

    ATMCard* card();
    QString bankName() const;
    size_t withdrawInterest() const;
    size_t transactInterest() const;
    QPixmap qrcode() const;

public slots:
    void insertCard(const QString&);
    void freeCard();
    void validatePin(const size_t);
    void changePin(const size_t);
    void sendToCard(const QString&, const size_t);
    void checkBal();
    void takeCash(const size_t);


signals:
    void atmStarted();
    void cardInserted();
    void cardFree();
    void qrSuccess();
    void pinValidated(const size_t);
    void pinSuccess();
    void pinChanged();
    void cashSend();
    void balChecked();
    void cashTaken(const long money);
    void errorOccured(const QString&);
};

#endif // ATM_H
