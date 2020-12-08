#include "atmtest.h"
#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include "ATM/Socket/atmselectorsocket.h"

ATMTest::ATMTest(QObject *parent) : QObject(parent),
    atm1_(Q_NULLPTR)
{
}

void ATMTest::testList()
{
    ATMSelectorSocket socket;
    QSignalSpy spy1(&socket, &ATMSelectorSocket::receivedATMParams);
    QSignalSpy spy2(&socket, &ATMSelectorSocket::errorOccured);
    QTest::qWait(2000);
    socket.askForATMParams();
    QTest::qWait(3000);
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 0);
}

void ATMTest::testStart()
{
    atm1_ = new ATM(3);
    QSignalSpy spy1(atm1_, &ATM::atmStarted);
    QSignalSpy spy2(atm1_, &ATM::errorOccured);
    QTest::qWait(3000);
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 0);
}

void ATMTest::testInsert()
{
    atm1_->insertCard("1111111111111112");
    QSignalSpy spy1(atm1_, &ATM::cardInserted);
    QSignalSpy spy2(atm1_, &ATM::errorOccured);
    QTest::qWait(3000);
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 0);
}

void ATMTest::testPin()
{
    QSignalSpy spy1(atm1_, SIGNAL(pinValidated(size_t)));
    QSignalSpy spy2(atm1_, &ATM::errorOccured);
    QSignalSpy spy3(atm1_, &ATM::pinSuccess);
    atm1_->validatePin("3333");
    QTest::qWait(2000);
    atm1_->validatePin("1111");
    QTest::qWait(2000);
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 1);
}

void ATMTest::testCheckBal()
{
    QSignalSpy spy1(atm1_, &ATM::balChecked);
    QSignalSpy spy2(atm1_, &ATM::errorOccured);
    atm1_->checkBal();
    QTest::qWait(2000);
    QVERIFY(atm1_->card() != Q_NULLPTR);
    QCOMPARE(spy2.count(), 0);
}

void ATMTest::testChangePin()
{
    QSignalSpy spy1(atm1_, &ATM::pinChanged);
    QSignalSpy spy2(atm1_, &ATM::errorOccured);
    atm1_->changePin("1112");
    QTest::qWait(2000);
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 0);
    atm1_->changePin("1111");
    QTest::qWait(2000);
    QCOMPARE(spy1.count(), 2);
    QCOMPARE(spy2.count(), 0);
}

void ATMTest::testTakeCash()
{
    QSignalSpy spy1(atm1_, &ATM::cashTaken);
    QSignalSpy spy2(atm1_, &ATM::errorOccured);
    atm1_->takeCash(10);
    QTest::qWait(3000);
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 0);
    ATMCard card(*atm1_->card());
    atm1_->checkBal();
    QTest::qWait(2000);
    QVERIFY(card.bal() == atm1_->card()->bal());
    QVERIFY(card.creditLim() == atm1_->card()->creditLim());
    QVERIFY(card.creditAval() == atm1_->card()->creditAval());
}

void ATMTest::testSendCash()
{
    QSignalSpy spy1(atm1_, &ATM::cashSend);
    QSignalSpy spy2(atm1_, &ATM::errorOccured);
    atm1_->sendToCard("1111111111111111", 10);
    QTest::qWait(3000);
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 0);
    ATMCard card(*atm1_->card());
    atm1_->checkBal();
    QTest::qWait(2000);
    QVERIFY(card.bal() == atm1_->card()->bal());
    QVERIFY(card.creditLim() == atm1_->card()->creditLim());
    QVERIFY(card.creditAval() == atm1_->card()->creditAval());
}

void ATMTest::testFreeCard()
{
    QSignalSpy spy1(atm1_, &ATM::cardFree);
    QSignalSpy spy2(atm1_, &ATM::errorOccured);
    atm1_->freeCard();
    QTest::qWait(3000);
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 0);
}



void ATMTest::cleanupTestCase()
{
    delete atm1_;
}


