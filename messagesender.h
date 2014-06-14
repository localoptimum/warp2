#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QObject>

class messageSender : public QObject
{
    Q_OBJECT
public:
    explicit messageSender(QObject *parent = 0);

    void uploadMessage(QString headerFileName, QString messageFileName, QString attachmentFileName);

signals:

public slots:
    void setUploadHeaderProgress(qint64 prog, qint64 progmax);
    void requestFinished(QNetworkReply *data);


private:
    QNetworkAccessManager *netManager;
    QNetworkReply *netReply;

    bool headerSendComplete;

};

#endif // MESSAGESENDER_H
