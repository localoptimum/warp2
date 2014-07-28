#include <iostream>
#include "mainwindow.h"
#include "keyListEditor.h"
#include "ui_mainwindow.h"
#include "ui_messageeditor.h"
#include "messagedelegate.h"
#include <QStandardPaths>
#include <QDir>
#include "message.h" //TESTING

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    messageEditor = new MessageEditor(this);
    keyListEditor = new KeyListEditor(this);


    //Initialise globals
    headerHash="";
    messageHash="";
    attachHash="";
    timestamp="";
    userPassword="";



    //find gpg2/gpg path
    gpgPath = QStandardPaths::findExecutable("gpg2", QStringList() << "/usr/local/bin/" << "/usr/bin/" << "/bin/" << "/usr/local/MacGPG2/bin/"); //H
    if(gpgPath.isEmpty()){
        //std::cout << "gpg2 not found" << std::endl;
        gpgPath = QStandardPaths::findExecutable("gpg", QStringList() << "/usr/local/bin/" << "/usr/bin/" << "/bin/");
    }
    if(gpgPath.isEmpty()){
        std::cerr << "gpg not found" << std::endl;
    }

    rootPath = QDir::homePath();
    rootPath.append("/.warp2/");

    ui->listWidget->setItemDelegate(new messagedelegate(ui->listWidget));
    ui->replyButton->setEnabled(false); //replybutton not clickable until message is selected
    loadMessages();
}

MainWindow::~MainWindow()
{
    delete ui;
}










QString MainWindow::getHeaderHash(void)
{
    return(headerHash);
}

QString MainWindow::getMessageHash(void)
{
    return(messageHash);
}

QString MainWindow::getAttachHash(void)
{
    return(attachHash);
}

QString MainWindow::getTimeStamp(void)
{
    return(timestamp);
}

QString MainWindow::getClearTextHash(void)
{
    return(clearTextHash);
}



void MainWindow::setHeaderHash(QString hash)
{
    headerHash = hash;
}

void MainWindow::setMessageHash(QString hash)
{
    messageHash = hash;
}

void MainWindow::setAttachHash(QString hash)
{
    attachHash = hash;
}

void MainWindow::setTimeStamp(QString stp)
{
    timestamp = stp;
}

void MainWindow::setClearTextHash(QString hash)
{
    clearTextHash = hash;
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
    //Get list of message headers from server since last timestamp

//    netmanager = new QNetworkAccessManager(this);
//    QObject::connect(netmanager, SIGNAL(finished(QNetworkReply*)),
//             this, SLOT(finishedSlot(QNetworkReply*)));

//    QUrl url("http://www.localoptimum.com/warp2/readInbox.php");
//    QNetworkReply* reply = netmanager->get(QNetworkRequest(url));



    //Try to decrypt each header in turn.  If a header decrypts successfully:
        //Add the message and attachment as necessary and store them locally.
        //Add a reference to them to the list of messages in the main display

    //Test newMsgHashes

    newMsgHashes = QStringList()
            << "39a8667f4f7b02bdf7607353bb1736de1b186ad9.warp2.header";

    QString decryptOutput;

    foreach(QString hdr, newMsgHashes)
    {
        decryptOutput = decryptHeader(QString(rootPath).append(hdr));

        if(decryptOutput != "")
        {
            //create new message and put in messages list
            std::cout << "output: " << decryptOutput.toStdString() <<  std::endl;
            QStringList headerContent = decryptOutput.split(QRegExp(";"));

            QString from = headerContent.filter("From:").takeAt(0).remove(0,5);
            std::cout << "From: " << from.toStdString() <<  std::endl;

            QString date = headerContent.filter("Date:").takeAt(0).remove(0,5);
            std::cout << "Date: " << date.toStdString() <<  std::endl;

            QString subject = headerContent.filter("Subject:").takeAt(0).remove(0,8);
            std::cout << "Subject: " << subject.toStdString() <<  std::endl;

            QString messageHash = headerContent.filter("MessageHash:").takeAt(0).remove(0,12);
            std::cout << "Message hash: " << messageHash.toStdString() << std::endl;

            QString attachmentHash = headerContent.filter("AttachmentHash:").takeAt(0).remove(0,15);
            std::cout << "Attachment Hash: " << attachmentHash.toStdString() << std::endl;

            // put string "Date:" before the date, so filter("Date:") is possible?


            //If we get here, this is great.  Decrypt the message, put a file:// link to the attachment, save as...

            QString messageHashLink = rootPath;
            messageHashLink.append(messageHash);
            messageHashLink.append(".warp2.message");

            std::cout << messageHashLink.toStdString() << std::endl;

            QString message = decryptMessage(messageHashLink);

            std::cout << "Message:" << std::endl << message.toStdString() << std::endl;

            QStringList messageLines = message.split("\n");
            QString firstLine = messageLines[0];

            if(firstLine.length()>20)
            {
                firstLine.truncate(20);
                firstLine.append("...");
            }

            if(message != "")
            {
                QString messageLink = messageHashLink;
                messageLink.append(".txt");
                addMessageToInbox(from, subject, date, firstLine, messageLink, attachmentHash);
            }
        }
    }

    //Refresh inbox
    loadMessages();
}


void MainWindow::downloadHeader(QString headerHash)
{

}



QString MainWindow::decryptHeader(QString headerHash)
{
    QProcess decryptProcess;
    QString decryptOutput;
    QString decryptError;
    QString gpgProcess = gpgPath;
    //ifdef linux..

    QFile headerHashFile(headerHash);

    if(!headerHashFile.exists())
    {
        std::cerr << "File not found " << headerHash.toStdString() << std::endl;
        return("");
    }

    decryptProcess.start(gpgPath,
                         QStringList() << "--batch"
                         << "-d"
                         << headerHash);

    decryptProcess.waitForFinished();

    decryptOutput = decryptProcess.readAllStandardOutput();
    decryptError = decryptProcess.readAllStandardError();

    decryptProcess.close();

    std::cerr << "error: " << decryptError.toStdString() << std::endl;

    if(decryptError.contains("no secret key", Qt::CaseInsensitive))
    {
        std::cout << "No secret key, mail not for you" << std::endl;
        return("");
    }
    else
    {
        return(decryptOutput);
    }
}





QString MainWindow::decryptMessage(QString messageHash)
{
    QProcess decryptProcess;
    QString decryptOutput;
    QString decryptError;
    QString gpgProcess = gpgPath;
    //ifdef linux..

    QFile messageHashFile(messageHash);

    if(!messageHashFile.exists())
    {
        std::cerr << "File not found " << messageHash.toStdString() << std::endl;
        return("");
    }

    QString clearFileName = messageHash;
    clearFileName.append(".txt");

    decryptProcess.start(gpgPath,
                         QStringList() << "--batch"
                         << "-d"
                         << messageHash
                         );

    decryptProcess.waitForFinished();

    decryptOutput = decryptProcess.readAllStandardOutput();
    decryptError = decryptProcess.readAllStandardError();

    decryptProcess.close();

    std::cerr << "error: " << decryptError.toStdString() << std::endl;

    if(decryptError.contains("no secret key", Qt::CaseInsensitive))
    {
        std::cout << "No secret key, mail not for you" << std::endl;
        return("");
    }
    else
    {
        QFile clearFile(clearFileName);
        clearFile.open(QIODevice::WriteOnly | QIODevice::Text);
        clearFile.write(decryptOutput.toLocal8Bit());

        clearFile.close();

        return(decryptOutput);
    }
}






void MainWindow::addMessageToInbox(QString from, QString subject, QString date, QString firstLine, QString messageLink, QString attachmentLink)
{
        //download and decrypt message and attachment
//        QString messageLink = QString("/Users/hannabjorgvinsdottir/Desktop/mail.txt"); //decryptMessage(messageHash);
//        QString attachmentLink = QString("/Users/hannabjorgvinsdottir/Desktop/skickamig.txt"); //decryptAttachment(attachmentHash);
//        QString date = "08-02-14";


        message mess = message(from, subject, date, firstLine, messageLink, attachmentLink); //testing
        this->messages.append(mess);
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





void MainWindow::loadMessages()
{
    //Create a QListWidgetItem for each message, and set appropriate data
    //If message has been read, set item->setData(Qt::UserRole+3, "read"),
    //and if not, anything but "read" dislpays as unread

    //Testing
    // QString header = QString("/private/tmp/e6aa3d26c8f3275d05c2761ddff875c1b3cf0256.warp2.header");
    // decryptHeader(header);
    //messages.append(message("Hanna@Warp2.net", "Hejsan", "10-02-14", "/Users/hannabjorgvinsdottir/Desktop/mail.txt", "/Users/hannabjorgvinsdottir/Desktop/skickamig.txt"));
    //messages.append(message("Lisa@Warp2.net", "Hey there", "12-02-14", "/Users/hannabjorgvinsdottir/Desktop/mail2.txt", ""));
    //messages.append(message("Emil@Warp2.net", "Hi Hanna", "09-04-14", "/Users/hannabjorgvinsdottir/Desktop/mail3.txt", ""));

    int i;
    QString read = QString("not read");  //flag message if not read
    for(i=0; i<messages.size(); i++){
//        if(i==3){ //just a test for visualisation
//            read = QString("read");    //flag message as read
//        }

        QListWidgetItem *item = new QListWidgetItem();
        message m = messages.at(i);
        item->setData(Qt::DisplayRole, m.getSender());
        item->setData(Qt::UserRole, m.getSubject());
        item->setData(Qt::UserRole + 1, m.getFirstLine()); //"Preview of email, stripped of newlines");
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
    if(!messageFile.exists())
    {
        std::cerr << "Message file " << messageFile.fileName().toStdString() << " does not exist in itemClicked function" << std::endl;
        return;
    }
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
