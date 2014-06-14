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

private:
    QNetworkAccessManager *netManager;
    QNetworkReply *netReply;

};

#endif // MESSAGESENDER_H
