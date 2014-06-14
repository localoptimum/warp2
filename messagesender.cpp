#include "messagesender.h"
#include "messageeditor.h"

#include <iostream>
#include <QFile>
#include <QUrl>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QHttpMultiPart>

messageSender::messageSender(QObject *parent) :
    QObject(parent),
    netManager(new QNetworkAccessManager)
{
    headerSendComplete = false;
}

void messageSender::uploadMessage(QString headerFileName, QString messageFileName, QString attachmentFileName)
{

//    //One solution online

//    QNetworkAccessManager networkManager;

//    QFile *headerFile = new QFile(headerFileName);
//    headerFile->open(QIODevice::ReadOnly);

//    QNetworkReply *reply = networkManager.post(QNetworkRequest(QUrl("http://192.168.1.3/warp2/sendmail.php") ), headerFile);
//    //headerFile->setParent(reply);





    //Another solution somewhere

//    QFile *headerFile = new QFile(headerFileName);

//    QUrl url("http://192.168.1.3/warp2/sendmail.php");
//        QNetworkRequest req(url);
//        req.setHeader(QNetworkRequest::ContentTypeHeader, tr("multipart/form-data"));

//    //QNetworkAccessManager * manager = new QNetworkAccessManager();
//        //connect(netManager, SIGNAL(finished(QNetworkReply*)), SLOT(requestFinished(QNetworkReply*)));
//        netReply = netManager->post(req, headerFile);
//        connect(netReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(setUploadHeaderProgress(qint64, qint64)), Qt::DirectConnection);

//        QEventLoop loop;
//        QObject::connect(netReply, SIGNAL(readyRead()), &loop, SLOT(quit()));

//        // Execute the event loop here, now we will wait here until readyRead() signal is emitted
//        // which in turn will trigger event loop quit.
//        loop.exec();

//        // Lets print the HTTP GET response.
//        qDebug( netReply->readAll());

//        std::cout << "Upload done" << std::endl;

    //Trying with multipart, the above simply hangs

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"uploadmessage\""));
    textPart.setBody("my text");

    QHttpPart headerPart;
    headerPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("QByteArray"));
    headerPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"uploadmessage\""));
    QFile *file = new QFile(headerFileName);
    file->open(QIODevice::ReadOnly);
    headerPart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

    multiPart->append(textPart);
    multiPart->append(headerPart);

    QUrl url("http://192.168.1.3/warp2/sendmail.php");
    QNetworkRequest request(url);

    QNetworkAccessManager manager;
    netReply = netManager->post(request, multiPart);
    multiPart->setParent(netReply); // delete the multiPart with the reply
    // here connect signals etc.

    connect(netReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(setUploadHeaderProgress(qint64, qint64)), Qt::DirectConnection);

    connect(netManager, SIGNAL(finished(QNetworkReply*)), SLOT(requestFinished(QNetworkReply*)));


    QEventLoop loop;
    QObject::connect(netReply, SIGNAL(readyRead()), &loop, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loop.exec();

    std::cout << "Upload done" << std::endl;


}

void messageSender::setUploadHeaderProgress(qint64 prog, qint64 progmax)
{
    qobject_cast<MessageEditor *>(parent())
     -> updateHeaderProgress( (int) ((float)prog / (float) progmax));
}



void messageSender::requestFinished(QNetworkReply *data) {
    // Lets print the HTTP GET response.
    qDebug( netReply->readAll());
}
