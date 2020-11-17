#ifndef ATMCARD_H
#define ATMCARD_H

#include <QJsonObject>

class ATMCard
{
private:
    long bal_;
    long creditAval_;
    long creditLim_;

public:

    long bal() const;
    long creditAval() const;
    long creditLim() const;


    static ATMCard fromJson(const QJsonObject&);

    ATMCard(const long, const long, const long);
    ~ATMCard();
    ATMCard(const ATMCard&);
    ATMCard& operator=(const ATMCard&);

};

#endif // ATMCARD_H
