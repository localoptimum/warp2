#include <iostream>
#include "mainwindow.h"
#include "keyListEditor.h"
#include "ui_mainwindow.h"
#include "ui_messageeditor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    messageEditor = new MessageEditor(this);
    keyListEditor = new KeyListEditor(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_mainCreateMessageButton_clicked()
{
    if(!messageEditor->isVisible()){
        messageEditor -> show();
        //std::cout << "not visible" << std::endl;
    }else{
        messageEditor->raise();
        messageEditor->activateWindow();
        //std::cout << "not active window" << std::endl;
    }
}

void MainWindow::on_mainContactsButton_clicked()
{
    if(!keyListEditor->isVisible()){
        keyListEditor -> show();
        //std::cout << "not visible" << std::endl;
    }else{
        keyListEditor->raise();
        keyListEditor->activateWindow();
        //std::cout << "not active window" << std::endl;
    }
}

void MainWindow::on_mainGetNewMessagesButton_clicked()
{

    //Need to make an inbox reader class
    //Need to make a class to download messages, headers and attachments - toggle http and dht within the class

    //Get list of message headers from server since last timestamp

    netmanager = new QNetworkAccessManager(this);
    QObject::connect(netmanager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(finishedSlot(QNetworkReply*)));

    QUrl url("http://192.168.1.66/warp2/readInbox.php");
    QNetworkReply* reply = netmanager->get(QNetworkRequest(url));
    // NOTE: Store QNetworkReply pointer (maybe into caller).
    // When this HTTP request is finished you will receive this same
    // QNetworkReply as response parameter.
    // By the QNetworkReply pointer you can identify request and response.







    //Try to decrypt each header in turn.  If a header decrypts successfully:
        //Add the message and attachment as necessary and store them locally.
        //Add a reference to them to the list of messages in the main display

}


void MainWindow::downloadHeader(QString headerHash)
{}

void MainWindow::decryptHeader(QString headerHash)
{}

void MainWindow::finishedSlot(QNetworkReply* reply)
{
    QString * serverReply;
    QByteArray bytes;

    int i;
    int idx;

    // Reading attributes of the reply
    // e.g. the HTTP status code
    QVariant statusCodeV =
    reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    // Or the target URL if it was a redirect:
    QVariant redirectionTargetUrl =
    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    // see CS001432 on how to handle this

    // no error received?
    if (reply->error() == QNetworkReply::NoError)
    {
        // read data from QNetworkReply here

        // Example 2: Reading bytes form the reply
        bytes = reply->readAll();  // bytes
        serverReply = new QString(bytes); // string

        std::cout << "Read " << bytes.size() << " bytes from inbox." << std::endl;

        std::cout << serverReply->toStdString() << std::endl;

        QStringList msgHashList = serverReply->split(QRegExp("\n\|\r\n\|\r"));

        newMsgHashes = msgHashList.filter(".header");

    }
    // Some http error received
    else
    {
        // handle errors here
        std::cout << "Errors in reading URL" << std::endl;

        std::cout << reply->errorString().toStdString() << std::endl;
    }

    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    reply->deleteLater();
}


// Add function when clicking on message to open a new message viewer with that message as the source.
