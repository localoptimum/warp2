#include "messagesender.h"
#include "messageeditor.h"
#include "warp2ServerURL.h"

#include <iostream>
#include <QFile>
#include <QFileInfo>
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
    uploadItem(headerFileName);
    setTotalSendProgress(50,100);

    uploadItem(messageFileName);
    setTotalSendProgress(75,100);

    if(attachmentFileName.isEmpty())
    {
        std::cout << "No attachment" << std::endl;
        setUploadAttachmentProgress(100, 100);
    }
    else
    {
        std::cout << "Uploading attachment \"" << attachmentFileName.toStdString() << "\"" << std::endl;
        uploadItem(attachmentFileName);
    }

    setTotalSendProgress(90,100);
    updateInbox(headerFileName, messageFileName, attachmentFileName);
    setTotalSendProgress(100,100);
}

void messageSender::uploadItem(QString itemName)
{


    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"uploadmessage\""));
    textPart.setBody("my text");

    QHttpPart headerPart;
    headerPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("QByteArray"));
    headerPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"uploadmessage\"; filename=\""+ itemName + "\""));
    QFile *file = new QFile(itemName);
    file->open(QIODevice::ReadOnly);
    headerPart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

    multiPart->append(textPart);
    multiPart->append(headerPart);

    QString sendURL = serverURL;
    sendURL.append("sendmail.php");

    QUrl url(sendURL);
    QNetworkRequest request(url);

    QNetworkAccessManager manager;
    netReply = netManager->post(request, multiPart);
    multiPart->setParent(netReply); // delete the multiPart with the reply
    // here connect signals etc.

    if(itemName.contains("message", Qt::CaseInsensitive))
    {
         connect(netReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(setUploadMessageProgress(qint64,qint64)), Qt::DirectConnection);
    }
    else if(itemName.contains("attachment", Qt::CaseInsensitive))
    {
         connect(netReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(setUploadAttachmentProgress(qint64,qint64)), Qt::DirectConnection);
    }
    else
    {
        connect(netReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(setUploadHeaderProgress(qint64,qint64)), Qt::DirectConnection);
    }


    connect(netManager, SIGNAL(finished(QNetworkReply*)), SLOT(requestFinished(QNetworkReply*)));


    QEventLoop loop;

    QObject::connect(netReply, SIGNAL(readyRead()), &loop, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loop.exec();

    std::cout << "Upload done" << std::endl;

    delete(netReply);

}

void messageSender::setUploadHeaderProgress(qint64 prog, qint64 progmax)
{
    qobject_cast<MessageEditor *>(parent())
     -> updateHeaderProgress( 100*(int) ((float)prog / (float) progmax));
}


void messageSender::setUploadMessageProgress(qint64 prog, qint64 progmax)
{
    qobject_cast<MessageEditor *>(parent())
     -> updateMessageProgress( 100*(int) ((float)prog / (float) progmax));
}


void messageSender::setUploadAttachmentProgress(qint64 prog, qint64 progmax)
{
    qobject_cast<MessageEditor *>(parent())
     -> updateAttachmentProgress( 100*(int) ((float)prog / (float) progmax));
}

void messageSender::setTotalSendProgress(qint64 prog, qint64 progmax)
{
    qobject_cast<MessageEditor *>(parent())
            ->updateTotalProgress( 100*(int) ((float)prog / (float) progmax));
}



void messageSender::requestFinished(QNetworkReply *data) {
    // Lets print the HTTP GET response.
    qDebug( netReply->readAll());
}


void messageSender::updateInbox(QString headerFileName, QString messageFileName, QString attachmentFileName)
{

    QString headerServer;
    QString messageServer;
    QString attachmentServer;

    QFileInfo headerFinfo(headerFileName);
    headerServer = headerFinfo.baseName();
    headerServer.append(".header");

    QFileInfo messageFinfo(messageFileName);
    messageServer = messageFinfo.baseName();
    messageServer.append(".message");

    QString urlBuild=serverURL;
    urlBuild.append("updateInbox.php?header=");
    urlBuild.append(headerServer);

    urlBuild.append("&message=");
    urlBuild.append(messageServer);

    urlBuild.append("&attachment=");

    if(!attachmentFileName.isEmpty())
    {
        QFileInfo attachmentFinfo(attachmentFileName);
        attachmentServer=attachmentFinfo.baseName();
        attachmentServer.append(".attachment");

        urlBuild.append(attachmentServer);
    }
    else
    {
        urlBuild.append("none");
    }

    QNetworkRequest request(urlBuild);

    QNetworkAccessManager manager;
    netReply = netManager->get(request);

    // here connect signals etc.

    connect(netManager, SIGNAL(finished(QNetworkReply*)), SLOT(requestFinished(QNetworkReply*)));


    QEventLoop loop;
    QObject::connect(netReply, SIGNAL(readyRead()), &loop, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loop.exec();

    std::cout << "Inbox rebuild done" << std::endl;
}
