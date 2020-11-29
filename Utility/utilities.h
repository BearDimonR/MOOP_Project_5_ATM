#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <QJsonDocument>
#include <QMap>
#include <QFile>

using namespace std;

class Utility
{
private:
    explicit Utility();

    Utility(const Utility&) = delete;
    Utility(Utility&&) = delete;
    Utility& operator=(const Utility&) = delete;
    Utility& operator=(Utility&&) = delete;

    QMap<QString, QVariant>* map_;

    QFile* logFile_;

    QVariant getVariable(const QString& name);

public:

    friend void log(QtMsgType type, const QMessageLogContext& log, const QString& msg);
    static Utility& getInstance();

    ~Utility();

    void askMap();
    void freeInstance();
    QString getString(const QString& name);
    QList<QString> getStringArr(const QString& name);

};

void log(QtMsgType type, const QMessageLogContext& log, const QString& msg);

#endif // UTILITIES_H
