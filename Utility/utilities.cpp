#include "utilities.h"
#include "ATM/clienterror.h"
#include <QFile>
#include <qiodevice.h>
#include <QTextStream>
#include <QJsonArray>
#include <QDebug>
#include <QDir>

Utility::Utility():
    map_(Q_NULLPTR)
{
    askMap();
}

Utility &Utility::getInstance()
{
    static Utility instance;
    return instance;
}

Utility::~Utility()
{
    freeInstance();
}

void Utility::askMap()
{
    QString path(QDir::currentPath() + "/config.json");
    QFile file(path);
	
    //qDebug() << QDir::currentPath();
    //QFile file("/Users/sofixeno/Desktop/Booblik/MOOP_Project_5_ATM/config.json");
    //QFile file(QDir::currentPath() + "/config.json");
    if (!file.open(QIODevice::ReadOnly))
        qFatal("%s", QString(ClientError("Utilities on open file error",
                                       ClientError::FILE_ERROR, path)).toLatin1().constData());
    QJsonParseError jsonError;
    QJsonDocument json = QJsonDocument::fromJson(file.readAll(),&jsonError);
    if (jsonError.error != QJsonParseError::NoError)
        qFatal("%s", QString(ClientError("Utilities parsing file error",
                                       ClientError::PARSING_ERROR, jsonError.errorString())).toLatin1().constData());
    map_ = new QMap<QString, QVariant>(json.toVariant().toMap());
    file.close();
}

void Utility::freeInstance()
{
    if(map_ != Q_NULLPTR)
    {
        delete map_;
        map_ = Q_NULLPTR;
    }
}

QVariant Utility::getVariable(const QString &name)
{
    if(map_ == Q_NULLPTR)
        askMap();
    if(!map_->contains(name))
        qFatal("%s", QString(ClientError("Utilities on getting variable",
                                       ClientError::FILE_ERROR, name)).constData());
    return (*map_)[name];
}

QString Utility::getString(const QString& name)
{
    return getVariable(name).toString();
}

QList<QString> Utility::getStringArr(const QString& name)
{
    QJsonArray arr(getVariable(name).toJsonArray());
    QList<QString> res;
    for(int i = 0; i< arr.size(); ++i)
        res.append(arr.at(i).toString());
    return res;
}
