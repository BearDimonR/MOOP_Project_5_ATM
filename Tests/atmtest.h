#ifndef ATMTEST_H
#define ATMTEST_H

#include <QObject>
#include "ATM/atm.h"

class ATMTest : public QObject
{
    Q_OBJECT

private:
    ATM* atm1_;
public:
    explicit ATMTest(QObject *parent = nullptr);


private slots:
    void testList();

    void testStart();
    void testInsert();
    void testPin();
    void testCheckBal();
    void testChangePin();
    void testTakeCash();
    void testSendCash();
    void testFreeCard();
    void cleanupTestCase();

};

#endif // ATMTEST_H
