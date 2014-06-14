#include "messagesender.h"
#include "sendprogressdialog.h"

#include <iostream>
#include <QFile>
#include <QUrl>
#include <QNetworkRequest>

messageSender::messageSender(QObject *parent) :
    QObject(parent),
    netManager(new QNetworkAccessManager)
{
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

    QFile *headerFile = new QFile(headerFileName);

    QUrl url("http://192.168.1.3/warp2/sendmail.php");
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, tr("multipart/form-data"));

    //QNetworkAccessManager * manager = new QNetworkAccessManager();
        connect(netManager, SIGNAL(finished(QNetworkReply*)), SLOT(SlotRequestFinished(QNetworkReply*)));
        netReply = netManager->post(req, headerFile);
        connect(netReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(setUploadHeaderProgress(qint64, qint64)), Qt::DirectConnection);

        //One proposed solution online is something like:
        //connect(static_cast<QObject*>(reply), SIGNAL(downloadProgress(qint64,qint64)), sendProgress, SLOT(setUploadHeaderProgress(qint64, qint64)));
        std::cout << "Upload done" << std::endl;

}

void messageSender::setUploadHeaderProgress(qint64 prog, qint64 progmax)
{
    qobject_cast<SendProgressDialog *>(parent())
     -> setUploadMessageProgress( (int) ((float)prog / (float) progmax));
}


