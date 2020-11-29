#include "appsocket.h"
#include "Utility/utilities.h"
#include "ATM/clienterror.h"
#include <QDebug>
#include <QTimer>
#include <QEventLoop>

QUrl AppSocket::HOST_URL = Utility::getInstance().getString("AppSocket_HOSTNAME");


void AppSocket::doOnConnected()
{
    qDebug() << "connected";
    connect(socket_, &QWebSocket::textMessageReceived,
            this, &AppSocket::onTextMessageReceived);
}

void AppSocket::doOnDisconnected()
{
    disconnect();
    qFatal("%s", QString(ClientError("Disconnected",
                                     ClientError::CONNECTION_ERROR, "Socket was disconnected")).toLatin1().constData());
}

void AppSocket::doOnSslErrors(const QList<QSslError> &errors)
{
    QString res;
    for(int i = 0; i < errors.size(); ++i)
        res += errors[i].errorString();
    qFatal("%s", QString(ClientError("SSL error",
                                     ClientError::CONNECTION_ERROR, res)).toLatin1().constData());
}

QJsonObject AppSocket::toJson(const QString & str)
{
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();
        }
        else
        {
            qFatal("%s", QString(ClientError("AppSocket to json error",
                                             ClientError::PARSING_ERROR, QJsonDocument(obj).toJson())).toLatin1().constData());
        }
    }
    else
    {
        qFatal("%s", QString(ClientError("AppSocket to json error",
                                         ClientError::PARSING_ERROR, QJsonDocument(obj).toJson())).toLatin1().constData());
    }

    return obj;
}

AppSocket::AppSocket(QObject *parent):
    QObject(parent),
    socket_(new QWebSocket())
{
    connect(socket_, &QWebSocket::connected, this, &AppSocket::onConnected);
    typedef void (QWebSocket:: *sslErrorsSignal)(const QList<QSslError> &);
    connect(socket_, static_cast<sslErrorsSignal>(&QWebSocket::sslErrors),
        this, &AppSocket::onSslErrors);
    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
    socket_->open(QUrl(HOST_URL));
}

AppSocket::~AppSocket()
{
    disconnect();
    socket_->close();
    delete socket_;
}

void AppSocket::sendMessage(const QString & event, const QString& content)
{
    QJsonObject payload;
    payload["content"] = content;
    QJsonObject message;
    message["event"] = event;
    message["payload"] = payload;
    QJsonDocument doc(message);
    QByteArray bytes = doc.toJson();
    socket_->sendTextMessage(QString(bytes));
}

void AppSocket::onError(QAbstractSocket::SocketError)
{
    qFatal("%s", QString(ClientError("Socket error",
                                     ClientError::CONNECTION_ERROR, socket_->errorString())).toLatin1().constData());
}
