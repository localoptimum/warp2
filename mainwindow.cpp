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

    //Load message-list file;
    //Load unread message list file;

    loadMessages();

    //Set messages that are on unread list as "unread"
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
    QString * serverReply;
    QByteArray bytes;

    //Get list of message headers from server since last timestamp

<<<<<<< HEAD
    netmanager = new QNetworkAccessManager(this);
    //QObject::connect(netmanager, SIGNAL(finished(QNetworkReply*)),
    //         this, SLOT(getInboxFinishedSlot(QNetworkReply*)));
=======
    QString * serverReply;
    QByteArray bytes;
    QString messageFileName;
    QString headerFileName;

    QStringList msgToDownload;

    netmanager = new QNetworkAccessManager(this);
    //QObject::connect(netmanager, SIGNAL(finished(QNetworkReply*)),
    //         this, SLOT(inboxListFinishedSlot(QNetworkReply*)));
>>>>>>> b5044ef90b9ceb82ff8849e3fa9ab59e2e44571d

    QUrl url("http://www.localoptimum.com/warp2/readInbox.php");
    QNetworkReply* reply = netmanager->get(QNetworkRequest(url));



    //Try to decrypt each header in turn.  If a header decrypts successfully:
    //Add the message and attachment as necessary and store them locally.
    //Add a reference to them to the list of messages in the main display

    QEventLoop loop;

    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    // Execute the event loop here, now we will wait here until finished() signal is emitted
    // which in turn will trigger event loop quit.
    loop.exec();

    reply->waitForReadyRead(500);

<<<<<<< HEAD
=======
    reply->waitForReadyRead(500);

>>>>>>> b5044ef90b9ceb82ff8849e3fa9ab59e2e44571d
    // Reading attributes of the reply
    // e.g. the HTTP status code
    QVariant statusCodeV =
    reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    // Or the target URL if it was a redirect:
    QVariant redirectionTargetUrl =
    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    // see CS001432 on how to handle this
<<<<<<< HEAD

    // no error received?
    if (reply->error() == QNetworkReply::NoError)
    {
        // read data from QNetworkReply here

        // Example 2: Reading bytes form the reply
        bytes = reply->readAll();  // bytes
        serverReply = new QString(bytes); // string

        std::cout << "Read " << bytes.size() << " bytes from inbox." << std::endl;

        std::cout << "Header download resulted in: " << serverReply->toStdString() << std::endl;

        //newMsgHashes = *serverReply;


    }
    // Some http error received
    else
    {
        // handle errors here
        std::cout << "Errors in reading URL" << std::endl;
        std::cout << reply->errorString().toStdString() << std::endl;

        return;
    }

    return;

        QStringList msgToDownload = newMsgHashes;

        foreach(QString hsh, msgToDownload)
        {
            std::cout << hsh.toStdString() << std::endl;
            downloadHeader(hsh);
        }

    //Test newMsgHashes

//    newMsgHashes = QStringList()
//            << "b8838a2e3869ed98bbc2f0e3c5c8fdbfee4215fd.warp2.header";
=======

    // no error received?
    if (reply->error() == QNetworkReply::NoError)
    {
        // read data from QNetworkReply here

        // Example 2: Reading bytes form the reply
        bytes = reply->readAll();  // bytes
        serverReply = new QString(bytes); // string

        std::cout << "Read " << bytes.size() << " bytes from inbox." << std::endl;

        //std::cout << serverReply->toStdString() << std::endl;

        QStringList msgHashList = serverReply->split(QRegExp("\n\|\r\n\|\r"));

        //newMsgHashes
        QStringList validMsgHashLines = msgHashList.filter(".header");


        foreach(QString s, validMsgHashLines)
        {
            QStringList sp = s.split(QRegExp(","));
            std::cout << sp[1].toStdString() << std::endl;
            //QString msgHeaderURL = "http://www.localoptimum.com/warp2/inbox/"

            //If we do not already have this header, add it to the list of headers to obtain
            //This needs to be more sophisticated, because we don't want to keep headers indefinitely...
            headerFileName = rootPath;
            headerFileName.append(sp[1]);
            std::cout << "Checking " << headerFileName.toStdString() << std::endl;

            if(!QFile(headerFileName).exists())
            {
                msgToDownload << sp[1];
            }
        }

    }
    // Some http error received
    else
    {
        // handle errors here
        std::cout << "Errors in reading URL" << std::endl;

        std::cout << reply->errorString().toStdString() << std::endl;
    }

    reply->deleteLater();




    if(msgToDownload.length()<1)
    {
        std::cout << "No new messages." << std::endl;
        return;
    }
    else
    {
        std::cout << "Downloading " << msgToDownload.length() << " headers." << std::endl;
    }

    //try to download the first one, just for fun
    //downloadHeader(newMsgHashes[0]);


    foreach(QString hsh, msgToDownload)
    {
        std::cout << hsh.toStdString() << std::endl;
        downloadHeader(hsh);
    }

    //Test newMsgHashes

    //newMsgHashes = QStringList()
    //        << "39a8667f4f7b02bdf7607353bb1736de1b186ad9.header";
>>>>>>> b5044ef90b9ceb82ff8849e3fa9ab59e2e44571d

    QString decryptOutput;

    foreach(QString hdr, msgToDownload)
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

            //First download the message

            messageFileName = messageHash;
            messageFileName.append(".message");
            downloadMessage(messageFileName);


            QString messageHashLink = rootPath;
            messageHashLink.append(messageHash);
            messageHashLink.append(".message");

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

    // Downloads a single header and saves it to disk, when provided with the headerhash.

    //Does the file exist already?  In which case, do not download it.
    QString localHeaderCopyFileName = QDir::homePath();
    localHeaderCopyFileName.append("/.warp2/");
    localHeaderCopyFileName.append(headerHash);

    QFile headerFile(localHeaderCopyFileName);

    if(headerFile.exists())
    {
        std::cout << localHeaderCopyFileName.toStdString() << " exists, skipping..." << std::endl;
        return;
    }
    else
    {
        std::cout << localHeaderCopyFileName.toStdString() << " downloading..." << std::endl;
    }



    QString * serverReply;
    QByteArray bytes;

    //std::cout << headerHash.toStdString() << std::endl;

    QString urlString="http://www.localoptimum.com/warp2/inbox/";
    urlString.append(headerHash);



    netmanager = new QNetworkAccessManager(this);

//    QObject::connect(netmanager, SIGNAL(finished(QNetworkReply*)),
//             this, SLOT(headerDownloadFinishedSlot(QNetworkReply*)));

    QUrl url(urlString);
    QNetworkReply* reply = netmanager->get(QNetworkRequest(url));

    // NOTE: Store QNetworkReply pointer (maybe into caller).
    // When this HTTP request is finished you will receive this same
    // QNetworkReply as response parameter.
    // By the QNetworkReply pointer you can identify request and response.


    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    std::cout << "Getting header " << headerHash.toStdString() << std::endl;

    loop.exec();


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

        std::cout << "Header download resulted in: " << serverReply->toStdString() << std::endl;

        //Write the header to file

        headerFile.open(QIODevice::WriteOnly | QIODevice::Text);
        headerFile.write(serverReply->toLocal8Bit());

        headerFile.close();

        std::cout << localHeaderCopyFileName.toStdString() << " download complete." << std::endl;

    }
    // Some http error received
    else
    {
        // handle errors here
        std::cout << "Errors in reading URL" << std::endl;

        std::cout << reply->errorString().toStdString() << std::endl;
    }

    reply->deleteLater();
}









void MainWindow::downloadMessage(QString messageHashFileName)
{

    // Downloads a single header and saves it to disk, when provided with the headerhash

    QStringList msgHashList = messageHashFileName.split(".");
    QString msgHash = msgHashList[0];

    //Does the file exist already?  In which case, do not download it.
    QString localMessageCopyFileName = QDir::homePath();
    localMessageCopyFileName.append("/.warp2/");
    localMessageCopyFileName.append(msgHash);
    localMessageCopyFileName.append(".message");

    QFile messageFile(localMessageCopyFileName);

    if(messageFile.exists())
    {
        std::cout << localMessageCopyFileName.toStdString() << " exists, skipping..." << std::endl;
        return;
    }
    else
    {
        std::cout << localMessageCopyFileName.toStdString() << " downloading..." << std::endl;
    }



    QString * serverReply;
    QByteArray bytes;

    //std::cout << headerHash.toStdString() << std::endl;

    QString urlString="http://www.localoptimum.com/warp2/inbox/";
    urlString.append(messageHashFileName);



    netmanager = new QNetworkAccessManager(this);

//    QObject::connect(netmanager, SIGNAL(finished(QNetworkReply*)),
//             this, SLOT(headerDownloadFinishedSlot(QNetworkReply*)));

    QUrl url(urlString);
    QNetworkReply* reply = netmanager->get(QNetworkRequest(url));

    // NOTE: Store QNetworkReply pointer (maybe into caller).
    // When this HTTP request is finished you will receive this same
    // QNetworkReply as response parameter.
    // By the QNetworkReply pointer you can identify request and response.


    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    std::cout << "Getting message " << messageHash.toStdString() << std::endl;

    loop.exec();


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

        std::cout << "Message download resulted in: " << serverReply->toStdString() << std::endl;

        //Write the message to file

        messageFile.open(QIODevice::WriteOnly | QIODevice::Text);
        messageFile.write(serverReply->toLocal8Bit());

        messageFile.close();

        std::cout << localMessageCopyFileName.toStdString() << " download complete." << std::endl;

    }
    // Some http error received
    else
    {
        // handle errors here
        std::cout << "Errors in reading URL" << std::endl;

        std::cout << reply->errorString().toStdString() << std::endl;
    }

    reply->deleteLater();
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





void MainWindow::headerDownloadFinishedSlot(QNetworkReply* reply)
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

        std::cout << "Header download resulted in: " << serverReply->toStdString() << std::endl;

        //Write the header to file




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




void MainWindow::inboxListFinishedSlot(QNetworkReply* reply)
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

        //std::cout << serverReply->toStdString() << std::endl;

        QStringList msgHashList = serverReply->split(QRegExp("\n\|\r\n\|\r"));

        //newMsgHashes
        QStringList validMsgHashLines = msgHashList.filter(".header");

        newMsgHashes.clear();

        foreach(QString s, validMsgHashLines)
        {
            QStringList sp = s.split(QRegExp(","));
            std::cout << sp[1].toStdString() << std::endl;
            //QString msgHeaderURL = "http://www.localoptimum.com/warp2/inbox/"

            newMsgHashes << sp[1];
        }

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
    // QString header = QString("/private/tmp/e6aa3d26c8f3275d05c2761ddff875c1b3cf0256.header");
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
