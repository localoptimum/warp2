#include <iostream>
#include "mainwindow.h"
#include "keyListEditor.h"
#include "ui_mainwindow.h"
#include "ui_messageeditor.h"
#include "messagedelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    messageEditor = new MessageEditor(this);
    keyListEditor = new KeyListEditor(this);
    ui->listWidget->setItemDelegate(new messagedelegate(ui->listWidget));
    ui->replyButton->setEnabled(false); //replybutton not clickable until message is selected
    loadMessages();
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

    QUrl url("http://www.localoptimum.com/warp2/readInbox.php");
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

void MainWindow::loadMessages(){
    //Create a QListWidgetItem for each message, and set appropriate data
    //If message has been read, set item->setData(Qt::UserRole+3, "read"),
    //and if not, anything but "read" dislpays as unread
    int i;
    QString read = QString("not read");
    for(i=0; i<20; i++){
        if(i==3){
            read = QString("read");
        }
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, "Sender name");
        item->setData(Qt::UserRole, "Subject");
        item->setData(Qt::UserRole + 1, "Preview of email, stripped of newlines");
        item->setData(Qt::UserRole + 2, "12-03-01");
        item->setData(Qt::UserRole + 3, read);
        ui->listWidget->addItem(item);
    }
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //Set the message contents as text in textEdit
    //and mark item as read
    ui->textEdit->setText("Hi there, \n\nLorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. \n\nLorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.  \n\nCheers!\nHanna");
    item->setData(Qt::UserRole + 3, QString("read"));
    //If reply button is disabled, enable it
    if(!ui->replyButton->isEnabled()){
        ui->replyButton->setEnabled(true);
    }
}

void MainWindow::on_replyButton_clicked()
{
    //show messageeditor, with sender of current email as recipient
}
