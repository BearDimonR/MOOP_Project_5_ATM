#ifndef APPSOCKET_H
#define APPSOCKET_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>

class AppSocket : public QObject
{

    Q_OBJECT

protected:

    static QUrl HOST_URL;

    virtual void doOnConnected();
    virtual void doOnDisconnected();
    virtual void doOnTextMessageReceived(const QJsonObject&) = 0;
    virtual void doOnSslErrors(const QList<QSslError> &errors);

    QJsonObject toJson(const QString&);

    AppSocket(const AppSocket&) = delete;
    AppSocket(AppSocket&&) = delete;
    AppSocket& operator=(const AppSocket&) = delete;
    AppSocket& operator=(AppSocket&&) = delete;

public:
    explicit AppSocket(QObject *parent = Q_NULLPTR);

    virtual ~AppSocket();

    void sendMessage(const QString&, const QString&);

public slots:
    void onError(QAbstractSocket::SocketError);
    void onConnected() { doOnConnected(); };
    void onDisconnected() { doOnDisconnected(); };
    void onTextMessageReceived(QString message) { doOnTextMessageReceived(toJson(message)); };
    void onSslErrors(const QList<QSslError> &errors) { doOnSslErrors(errors); };

protected:
    QWebSocket* socket_;
};

#endif // APPSOCKET_H
