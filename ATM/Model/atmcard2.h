#ifndef ATMCARD_H
#define ATMCARD_H

#include <QJsonObject>

class ATMCard
{
private:
    float bal_;
    float creditAval_;
    float creditLim_;
    size_t with_int_;
    size_t trans_int_;

    ATMCard(const float, const float, const float, const size_t, const size_t);
public:

    float bal() const;
    float creditAval() const;
    float creditLim() const;
    size_t withdraw() const;
    size_t transact() const;

    static ATMCard fromJson(const QJsonObject&);

    void updateBal(const float, const float, const float);

    ~ATMCard();
    ATMCard(const ATMCard&);
    ATMCard& operator=(const ATMCard&);

};

#endif // ATMCARD_H
