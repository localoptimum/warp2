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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_mainCreateMessageButton_clicked()
{
    messageEditor = new MessageEditor(this);
    messageEditor -> show();
}

void MainWindow::on_mainContactsButton_clicked()
{
    keyListEditor = new KeyListEditor(this);
    keyListEditor -> show();
}

void MainWindow::on_mainGetNewMessagesButton_clicked()
{
    //Get list of message headers from server since last timestamp

    netmanager = new QNetworkAccessManager(this);
    QObject::connect(netmanager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(finishedSlot(QNetworkReply*)));

    QUrl url("http://www.google.se");
    QNetworkReply* reply = netmanager->get(QNetworkRequest(url));
    // NOTE: Store QNetworkReply pointer (maybe into caller).
    // When this HTTP request is finished you will receive this same
    // QNetworkReply as response parameter.
    // By the QNetworkReply pointer you can identify request and response.







    //Try to decrypt each header in turn.  If a header decrypts successfully:
        //Add the message and attachment as necessary and store them locally.
        //Add a reference to them to the list of messages in the main display



}

void MainWindow::finishedSlot(QNetworkReply* reply)
{
    QString * serverReply;
    QByteArray bytes;

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
