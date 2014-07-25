#include <iostream>
#include "mainwindow.h"
#include "keyListEditor.h"
#include "ui_mainwindow.h"
#include "ui_messageeditor.h"
#include "messagedelegate.h"
#include <QStandardPaths>
#include "message.h" //TESTING

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    messageEditor = new MessageEditor(this);
    keyListEditor = new KeyListEditor(this);

    //find gpg2/gpg path
    gpgPath = QStandardPaths::findExecutable("gpg2", QStringList() << "/usr/local/bin/" << "/usr/bin/" << "/bin/" << "/usr/local/MacGPG2/bin/"); //H
    if(gpgPath.isEmpty()){
        //std::cout << "gpg2 not found" << std::endl;
        gpgPath = QStandardPaths::findExecutable("gpg", QStringList() << "/usr/local/bin/" << "/usr/bin/" << "/bin/");
    }
    if(gpgPath.isEmpty()){
        std::cerr << "gpg not found" << std::endl;
    }

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
{
    QProcess decryptProcess;
    QString decryptOutput;
    QString decryptError;
    QString gpgProcess = gpgPath;
    //ifdef linux..
    gpgProcess.append(" --batch -d ").append(headerHash);
    decryptProcess.start(gpgProcess);
    decryptProcess.waitForFinished();

    decryptOutput = decryptProcess.readAllStandardOutput();
    decryptError = decryptProcess.readAllStandardError();

    decryptProcess.close();

    std::cout << "error: " << decryptError.toStdString() << std::endl;
    if(decryptError.contains("no secret key", Qt::CaseInsensitive)){
        std::cout << "No secret key, mail not for you" << std::endl;
    }else{
        //create new message and put in messages list
        std::cout << "output: " << decryptOutput.toStdString() <<  std::endl;
        QStringList headerContent = decryptOutput.split(QRegExp(";"));
        QString from = headerContent.filter("From:").takeAt(0).remove(0,5);
        QString subject = headerContent.filter("Subject:").takeAt(0).remove(0,8);
        QString messageHash = headerContent.filter("Message:").takeAt(0).remove(0,8);
        QString attachmentHash = headerContent.filter("Attachment:").takeAt(0).remove(0,11);
        // put string "Date:" before the date, so filter("Date:") is possible?

        std::cout << "From: " << from.toStdString() <<  std::endl;
        std::cout << "Subject: " << subject.toStdString() <<  std::endl;
        std::cout << "Message hash: " << messageHash.toStdString() << std::endl;
        std::cout << "Attachment hash: " << attachmentHash.toStdString() << std::endl;

        //download and decrypt message and attachment
        QString messageLink = QString("/Users/hannabjorgvinsdottir/Desktop/mail.txt"); //decryptMessage(messageHash);
        QString attachmentLink = QString("/Users/hannabjorgvinsdottir/Desktop/skickamig.txt"); //decryptAttachment(attachmentHash);
        message mess = message(from, subject, QString("08-02-14"), messageLink, attachmentLink); //testing
        this->messages.append(mess);
    }
}

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

    //Testing
    // QString header = QString("/private/tmp/e6aa3d26c8f3275d05c2761ddff875c1b3cf0256.warp2.header");
    // decryptHeader(header);
    messages.append(message("Hanna@Warp2.net", "Hejsan", "10-02-14", "/Users/hannabjorgvinsdottir/Desktop/mail.txt", "/Users/hannabjorgvinsdottir/Desktop/skickamig.txt"));
    messages.append(message("Lisa@Warp2.net", "Hey there", "12-02-14", "/Users/hannabjorgvinsdottir/Desktop/mail2.txt", ""));
    messages.append(message("Emil@Warp2.net", "Hi Hanna", "09-04-14", "/Users/hannabjorgvinsdottir/Desktop/mail3.txt", ""));
    int i;
    QString read = QString("not read");
    for(i=0; i<messages.size(); i++){
        if(i==3){
            read = QString("read");
        }
        QListWidgetItem *item = new QListWidgetItem();
        message m = messages.at(i);
        item->setData(Qt::DisplayRole, m.getSender());
        item->setData(Qt::UserRole, m.getSubject());
        item->setData(Qt::UserRole + 1, "Preview of email, stripped of newlines");
        item->setData(Qt::UserRole + 2, m.getDate());
        item->setData(Qt::UserRole + 3, read);
        ui->listWidget->addItem(item);
    }
    //std::cout << "Messages count: " << messages.count() << std::endl;
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //Get messagecontent from messages.at(ui->listWidget->currentRow()).getMessageLink();
    //Load the content into textEdit, and mark item as read
    message mess = messages.at(ui->listWidget->currentRow());
    QString messageLink = mess.getMessageLink();
    QFile messageFile(messageLink);
    if(!messageFile.open(QIODevice::ReadOnly)){
        return; //or something
    }
    QByteArray mc = messageFile.readAll();
    QString messageContent = (QString) mc;
    if(!mess.getAttachmentLink().isEmpty()){
        messageContent.append("\n\nAttachment");
    }
    //QString text = QString("Message nbr ").append(QString::number(ui->listWidget->currentRow()));
    ui->textEdit->setText(messageContent);
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
