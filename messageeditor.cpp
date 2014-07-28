#include "messageeditor.h"
#include "ui_messageeditor.h"
#include "messagesender.h"
#include "mainwindow.h"

#include "sendprogressdialog.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QProcess>
#include <QDateTime>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTemporaryFile>


#include <iostream>

MessageEditor::MessageEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageEditor)
{
    int i;

    ui->setupUi(this);

    //find gpg2/gpg path
    gpgPath = QStandardPaths::findExecutable("gpg2", QStringList() << "/usr/local/bin/" << "/usr/bin/" << "/bin/" << "/usr/local/MacGPG2/bin/");
    if(gpgPath.isEmpty()){
        //std::cout << "gpg2 not found" << std::endl;
        gpgPath = QStandardPaths::findExecutable("gpg", QStringList() << "/usr/local/bin/" << "/usr/bin/" << "/bin/");
    }
    if(gpgPath.isEmpty()){
        std::cerr << "gpg not found" << std::endl;
    }

    //find sha/sha1 path
    shaPath = QStandardPaths::findExecutable("sha1sum", QStringList() << "/usr/local/bin/" << "/usr/bin/" << "/bin/");
    if(shaPath.isEmpty()){
        //On apple systems sha1sum is called "shasum" instead, try that:
        shaPath = QStandardPaths::findExecutable("shasum", QStringList() << "/usr/local/bin/" << "/usr/bin/" << "/bin/");
    }
    if(shaPath.isEmpty()){
        std::cerr << "sha not found" << std::endl;
    }
    //std::cout << "Getting keys" << std::endl;

    QProcess gpgGetKeys;
    QProcess gpgGetSecretKeys;



    // GET THE PUBLIC KEYS OF ALL KNOWN CONTACTS

    //gpgGetKeys.start("gpg  --list-keys");

    gpgGetKeys.start(gpgPath, QStringList() << "--list-keys");

    gpgGetKeys.setProcessChannelMode(QProcess::ForwardedChannels);

    gpgGetKeys.waitForFinished();

    QString output(gpgGetKeys.readAllStandardOutput());
    QString errorout(gpgGetKeys.readAllStandardError());


    QStringList templist1, templist2;

    QStringList linesplit = output.split(QRegExp("\>|\<"));
    QStringList addresses = linesplit.filter(QRegExp(".*@.*"));

    addresses.removeDuplicates();

    for(i=0; i<addresses.size(); i++)
        {
            //linesplitTrimmed.append(potentialKeyPairs.at(i).simplified());
            //std::cout << addresses.at(i).toStdString() << std::endl;
         ui->addresseePullDown->addItem(addresses.at(i));
        }



    // GET MY PRIVATE KEY

    //gpgGetSecretKeys.start("gpg  --list-secret-keys");

    gpgGetSecretKeys.start(gpgPath, QStringList() << "--list-secret-keys");

    gpgGetSecretKeys.setProcessChannelMode(QProcess::ForwardedChannels);

    gpgGetSecretKeys.waitForFinished();

    output = gpgGetSecretKeys.readAllStandardOutput();

    linesplit = output.split(QRegExp("\>|\<"));
    QStringList fromaddresses = linesplit.filter(QRegExp(".*@.*"));

    fromaddresses.removeDuplicates();

    for(i=0; i<fromaddresses.size(); i++)
        {
            //linesplitTrimmed.append(potentialKeyPairs.at(i).simplified());
            //std::cout << fromaddresses.at(i).toStdString() << std::endl;
            ui->fromPullDown->addItem(fromaddresses.at(i));
        }

}

MessageEditor::~MessageEditor()
{
    delete ui;
}

void MessageEditor::on_messageWindowCancelButton_clicked()
{
     QWidget::close();
}

void MessageEditor::on_attachmentFileBrowseButton_clicked()
{

    attachmentFileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)"));

    ui->attachmentFileBox->setText(attachmentFileName);
}


void MessageEditor::on_messageWindowSendButton_clicked()
{


    // Reset file paths so errors can be tracked
    attachmentFileName = "";
    headerFileName = "";
    messageFileName = "";


    //Present send progress

    sendProgress = new SendProgressDialog(this);
    myMessageSender = new messageSender(this);

    sendProgress -> show();

    sendProgress->setTotalProgress(0);
    sendProgress->setAssembleFilesProgress(0);
    sendProgress->setUploadHeaderProgress(0);
    sendProgress->setUploadMessageProgress(0);
    sendProgress->setUploadAttachmentProgress(0);


    this->assembleMessage();

    this->assembleAttachment();

    this->assembleHeader();

    //sendProgress->setTotalProgress(25);


    myMessageSender->uploadMessage(headerFileName,messageFileName,attachmentFileName);



//    QTemporaryFile messageTemp;

//    messageTemp.open();

//    QString messageTempFileName = messageTemp.fileName();

//    messageTemp.write(messageBodyText.toLocal8Bit());
//    messageTemp.setAutoRemove(false);  // we will manually remove the temp files

//    messageTemp.close();



    //Encrypt message body
    //Make tempfile of body
    //Sha1 body


    //Even if attachment is not empty, then still:
    //Encrypt attachment
    //Make tempfile of attachment
    //Sha1 attachment


    //Compute timestamp token
    //Upload header to server

    //Compute timestamp token
    //Upload message to server

    //Compute timestamp token
    //Upload attachment to server

    //Clean up - remove temporary files etc

    std::cout << "Message sent" << std::endl;

    sendProgress->setTotalProgress(100);
    sendProgress->close();

    delete myMessageSender;

    QWidget::close();
}
























void MessageEditor::assembleHeader(void)
{
    //Assemble header material into header string


    QString msgHeader;
    QString hsh;

    msgHeader = "Warp2Header;Version:Beta;";
    msgHeader.append("From:");
    msgHeader.append(ui->fromPullDown->currentText());
    msgHeader.append(";");

    msgHeader.append("To:");
    msgHeader.append(ui->addresseePullDown->currentText());
    msgHeader.append(";");

    msgHeader.append("Date:");
    QDateTime rightnow(QDateTime::currentDateTime());
    msgHeader.append(rightnow.toString());
    msgHeader.append(";");

    msgHeader.append("Subject:");
    msgHeader.append(ui->subjectTextBox->text());
    msgHeader.append(";");

    msgHeader.append("MessageHash:");
    hsh = ((MainWindow*)parent())->getMessageHash();
    if(hsh=="")
    {
        std::cerr << "No valid message hash in header assembly." << std::endl;
        return;
    }
    msgHeader.append(hsh);
    msgHeader.append(";");

    msgHeader.append("AttachmentHash:");
    hsh = ((MainWindow*)parent())->getAttachHash();
    if(hsh == "")
    {
        msgHeader.append(hsh);
    }
    else
    {
        msgHeader.append("None");
    }


//    std::cout << msgHeader.toStdString() << std::endl;



    //Make tempfile of header

    QTemporaryFile headerTemp;

    headerTemp.open();
    headerTemp.setAutoRemove(false);  // we will manually remove the temp files after send process is finished

    QString headerTempFileName = headerTemp.fileName();

    headerTemp.write(msgHeader.toLocal8Bit());

    headerTemp.close();




    //Encrypt header file

    QProcess encryptProcess;
    QString encryptOutput;
    QString encryptError;

//    QString encryptPipe = "gpg -a -e -u ";
//    encryptPipe.append(ui->fromPullDown->currentText());

//    QString encryptPipe = "--batch -a -e -r ";
//    encryptPipe.append(ui->addresseePullDown->currentText());
//    encryptPipe.append(" >> ");
//    encryptPipe.append(headerTempFileName);


//    std::cout << encryptPipe.toStdString() << std::endl;

    encryptProcess.start(gpgPath, QStringList()
                         << "--batch"
                         << "-a"
                         << "-e"
                         << "-r"
                         << ui->addresseePullDown->currentText()
                         << headerTempFileName
                         );

    encryptProcess.setProcessChannelMode(QProcess::ForwardedChannels);

//    encryptProcess.write(msgHeader.toLocal8Bit()); // then wait for bytes written before reading

//    encryptProcess.waitForBytesWritten();
//    encryptProcess.closeWriteChannel();
    encryptProcess.waitForFinished();

//    encryptOutput = encryptProcess.readAllStandardOutput();
    encryptError = encryptProcess.readAllStandardError();


    if(encryptError.size() > 0)
    {
        QMessageBox::critical(
              this,
              "Critical Error",
              encryptError);
    }

//    std::cout << "Encrypt Output:" << std::endl << encryptOutput.toStdString() << std::endl;
//    std::cout << "Encrypt Error:" << std::endl << encryptError.toStdString() << std::endl;

    encryptProcess.close();

    QFile headerEncryptedAsc(headerTempFileName.append(".asc"));

    QString headerEncryptedFileName;

    if(!headerEncryptedAsc.exists())
    {
        std::cerr << "Encrypted header file " << headerTempFileName.toStdString() << "  not successfully created.";
        return;
    }
    else
    {
        headerEncryptedFileName = headerEncryptedAsc.fileName();
    }








    //Sha1 header

    QProcess sha1process;

    sha1process.start("sha1sum", QStringList() << headerEncryptedFileName);

    sha1process.setProcessChannelMode(QProcess::ForwardedChannels);

    sha1process.waitForFinished();

    QString headerSha1(sha1process.readAllStandardOutput());
    QString sha1err(sha1process.readAllStandardError());

    if(sha1err.size() > 0)
    {
        QMessageBox::critical(
              this,
              "Critical Error",
              sha1err);
    }

    headerSha1 = headerSha1.left(40);

//    std::cout << "sha1 Output:" << std::endl << headerSha1.toStdString() << std::endl;
//    std::cout << "sha1 Error:" << std::endl << sha1err.toStdString() << std::endl;

    sha1process.close();

    QProcess fileRenameProcess;
    QString headerSha1Filename = QString("/tmp/").append(headerSha1).append(".warp2.header");  //this is sloppy, need to find directory of original file dynamically
    QString renamePipe = "mv ";
    renamePipe.append(headerEncryptedFileName);
    renamePipe.append(" ");
    renamePipe.append(headerSha1Filename);


//    std::cout << headerTempName.toStdString() << " " << headerSha1Filename.toStdString() << std::endl;

    fileRenameProcess.start("/bin/bash", QStringList() << "-c" << renamePipe);

    fileRenameProcess.setProcessChannelMode(QProcess::ForwardedChannels);
    fileRenameProcess.waitForFinished();

//    QString renameOutput = fileRenameProcess.readAllStandardOutput();
    QString renameError = fileRenameProcess.readAllStandardError();

    if(renameError.size() > 0)
    {
        QMessageBox::critical(
              this,
              "Critical Error",
              renameError);
    }
    else
    {
        headerFileName = headerSha1Filename;
        sendProgress->setAssembleFilesProgress(33);
    }
//    std::cout << "Rename Output:" << std::endl << renameOutput.toStdString() << std::endl;
//    std::cout << "Rename Error:" << std::endl << renameError.toStdString() << std::endl;
}






























void MessageEditor::assembleMessage(void)
{


    QString messageBodyText = ui->messageTextBox->toPlainText();

    QByteArray hash = QCryptographicHash::hash(messageBodyText.toLocal8Bit(), QCryptographicHash::Sha1);

    messageComposeToken = hash.toHex();

    ((MainWindow*)parent()) -> setClearTextHash(messageComposeToken);

    std::cout << "Message:" << std::endl;

    //Line endings need to be sorted out for different platforms - linux, windows etc.  Double check the hashes
    //On linux, to get the same hash as in here, you need to do echo -n "string" | sha1sum, if you omit the "-n" then echo puts \n on the end!
    std::cout << messageBodyText.toStdString() << std::endl;

    std::cout << "Compose token:" << std::endl;

    std::cout << messageComposeToken.toStdString() << std::endl;


    //Make tempfile of msg

    QTemporaryFile messageTemp;

    messageTemp.open();
    messageTemp.setAutoRemove(false);  // we will manually remove the temp files after send process is finished
    messageTemp.write(messageBodyText.toLocal8Bit()); // write message to temp file
    QString messageTempFileName = messageTemp.fileName();

    messageTemp.close();

    //Encrypt message

    QProcess encryptProcess;
    QString encryptOutput;
    QString encryptError;

    encryptProcess.start(gpgPath, QStringList()
                         << "--batch"
                         << "-a"
                         << "-e"
                         << "-r"
                         << ui->addresseePullDown->currentText()
                         << messageTempFileName
                         );

//    QString encryptPipe = "gpg -e -u ";
//    encryptPipe.append(ui->fromPullDown->currentText());

    /*** Replaced by encryptProcess.start(gpgPath,...)
    QString encryptPipe = "gpg -a -e -r ";
    encryptPipe.append(ui->addresseePullDown->currentText());
    encryptPipe.append(" >> ");
    encryptPipe.append(messageTempName);
    */
//    std::cout << encryptPipe.toStdString() << std::endl;

    //encryptProcess.start("/bin/bash", QStringList() << "-c" << encryptPipe);

//#if defined Q_OS_MAC
//    // Can run gpg directly, system provides stdin and stdout

//    QString gpgProcess = gpgPath;
//    gpgProcess.append(" -a -r ").append(ui->addresseePullDown->currentText()).append(" -e ").append(messageTempName);

//#elif defined Q_OS_WIN32
//    //windows specific code for launching gpg via cmd


//#else
//    //assume linux or similar tty & x11 system - this needs to run via /bin/bash to get access to stdin and stdout
//    QString gpgProcess = "/bin/bash -c ";
//    gpgProcess.append(gpgPath);
//    gpgProcess.append(" -a -r ").append(ui->addresseePullDown->currentText()).append(" -e ").append(messageTempName);
//#endif




//    QString messageTempNameAsc = messageTempName;
//    messageTempNameAsc.append(".asc");

//    std::cout << gpgProcess.toStdString() << std::endl;

//    encryptProcess.start(gpgProcess);


    //encryptProcess.start(gpgPath, QStringList() << "-c" << "-a" << "-r" << ui->addresseePullDown->currentText() << "-e" << messageTempName);


/* This bit itsn't working
    encryptProcess.setProcessChannelMode(QProcess::ForwardedChannels);

    encryptProcess.write(messageBodyText.toLocal8Bit()); // then wait for bytes written before reading

    encryptProcess.waitForBytesWritten();
    encryptProcess.closeWriteChannel();*/


    encryptProcess.waitForFinished();

//    encryptOutput = encryptProcess.readAllStandardOutput();
    encryptError = encryptProcess.readAllStandardError();


    if(encryptError.size() > 0)
    {
        QMessageBox::critical(
              this,
              "Critical Error",
              encryptError);
    }

//    std::cout << "Encrypt Output:" << std::endl << encryptOutput.toStdString() << std::endl;
//    std::cout << "Encrypt Error:" << std::endl << encryptError.toStdString() << std::endl;

    encryptProcess.close();

    QFile messageEncryptedAsc(messageTempFileName.append(".asc"));

    QString messageEncryptedFileName;

    if(!messageEncryptedAsc.exists())
    {
        std::cerr << "Encrypted message file " << messageTempFileName.toStdString() << "  not successfully created.";
        return;
    }
    else
    {
        messageEncryptedFileName = messageEncryptedAsc.fileName();
    }
















    //Sha1 hash the message

    QProcess sha1process;

    //sha1process.start("sha1sum", QStringList() << messageTempName);
    sha1process.start(shaPath, QStringList() << messageEncryptedFileName);

    sha1process.setProcessChannelMode(QProcess::ForwardedChannels);

    sha1process.waitForFinished();

    QString messageSha1(sha1process.readAllStandardOutput());
    QString sha1err(sha1process.readAllStandardError());

    if(sha1err.size() > 0)
    {
        QMessageBox::critical(
              this,
              "Critical Error",
              sha1err);
    }

    // Trim the hash info to the actual 40-character hash.  The other part is the filename or "-" if it was stdin
    messageSha1 = messageSha1.left(40);

//    std::cout << "sha1 Output:" << std::endl << messageSha1.toStdString() << std::endl;
//    std::cout << "sha1 Error:" << std::endl << sha1err.toStdString() << std::endl;

    sha1process.close();

    QProcess fileRenameProcess;
    QString messageSha1Filename = QString("/tmp/").append(messageSha1).append(".warp2.message");  //this is sloppy, need to find directory of original file dynamically
    QString renamePipe = "mv ";
    renamePipe.append(messageEncryptedFileName);
    renamePipe.append(" ");
    renamePipe.append(messageSha1Filename);


//    std::cout << messageTempName.toStdString() << " " << messageSha1Filename.toStdString() << std::endl;

    fileRenameProcess.start("/bin/bash", QStringList() << "-c" << renamePipe);

    fileRenameProcess.setProcessChannelMode(QProcess::ForwardedChannels);
    fileRenameProcess.waitForFinished();

//    QString renameOutput = fileRenameProcess.readAllStandardOutput();
    QString renameError = fileRenameProcess.readAllStandardError();

    if(renameError.size() > 0)
    {
        QMessageBox::critical(
              this,
              "Critical Error",
              renameError);
    }
    else
    {
        messageFileName = messageSha1Filename;
        ((MainWindow*)parent())->setMessageHash(messageSha1);
        sendProgress->setAssembleFilesProgress(66);

    }
//    std::cout << "Rename Output:" << std::endl << renameOutput.toStdString() << std::endl;
//    std::cout << "Rename Error:" << std::endl << renameError.toStdString() << std::endl;
}

























void MessageEditor::assembleAttachment(void)
{

    attachmentFileName = ui->attachmentFileBox->text();

    if(attachmentFileName.isEmpty() || !QFileInfo(attachmentFileName).exists())
    {
        std::cout << "No attachment to assemble." << std::endl;
        attachmentFileName = "";
        sendProgress->setAssembleFilesProgress(100);
        return;
    }



    //Encrypt message

    QProcess encryptProcess;
    QString encryptOutput;
    QString encryptError;

//    QString encryptPipe = "gpg -e -u ";
//    encryptPipe.append(ui->fromPullDown->currentText());

    /*** Replaced by encryptProcess.start(gpgPath,...)
    QString encryptPipe = "gpg -r ";
    encryptPipe.append(ui->addresseePullDown->currentText());
    encryptPipe.append(" --yes -a -e ");
    encryptPipe.append(attachmentFileName);

    std::cout << encryptPipe.toStdString() << std::endl;
    */

    //encryptProcess.start("/bin/bash", QStringList() << "-c" << encryptPipe);
    //encryptProcess.start(gpgPath, QStringList() << "-c" << "-r" << ui->addresseePullDown->currentText() << "--yes" << "-a" << "-e" << attachmentFileName);


#if defined Q_OS_MAC
    // Can run gpg directly, system provides stdin and stdout

    QString gpgProcess = gpgPath;
    gpgProcess.append(" -a -r ").append(ui->addresseePullDown->currentText()).append(" -e ").append(attachmentFileName);

#elif defined Q_OS_WIN32
    //windows specific code for launching gpg via cmd


#else
    //assume linux or similar tty & x11 system - this needs to run via /bin/bash to get access to stdin and stdout
    QString gpgProcess = "/bin/bash -c ";
    gpgProcess.append(gpgPath);
    gpgProcess.append(" -a -r ").append(ui->addresseePullDown->currentText()).append(" -e ").append(attachmentFileName);
#endif


    QString attachmentFileNameAsc = attachmentFileName;
    attachmentFileNameAsc.append(".asc");

    std::cout << gpgProcess.toStdString() << std::endl;

    encryptProcess.start(gpgProcess);



    encryptProcess.setProcessChannelMode(QProcess::ForwardedChannels);

    encryptProcess.waitForFinished();

//    encryptOutput = encryptProcess.readAllStandardOutput();
    encryptError = encryptProcess.readAllStandardError();

    if(encryptError.size() > 0)
    {
        QMessageBox::critical(
              this,
              "Critical Error",
              encryptError);
        return;
    }

    //GEt here, encryption worked


//    std::cout << "Encrypt Output:" << std::endl << encryptOutput.toStdString() << std::endl;
//    std::cout << "Encrypt Error:" << std::endl << encryptError.toStdString() << std::endl;

    encryptProcess.close();

    //Sha1 hash the message

    QProcess sha1process;

    //sha1process.start("sha1sum", QStringList() << attachmentFileName);
    sha1process.start(shaPath, QStringList() << attachmentFileNameAsc);

    sha1process.setProcessChannelMode(QProcess::ForwardedChannels);

    sha1process.waitForFinished();

    QString attachmentSha1(sha1process.readAllStandardOutput());
    QString sha1err(sha1process.readAllStandardError());

    if(sha1err.size() > 0)
    {
        QMessageBox::critical(
              this,
              "Critical Error",
              sha1err);
    }

    // Trim the hash info to the actual 40-character hash.  The other part is the filename or "-" if it was stdin
    attachmentSha1 = attachmentSha1.left(40);

//    std::cout << "sha1 Output:" << std::endl << attachmentSha1.toStdString() << std::endl;
//    std::cout << "sha1 Error:" << std::endl << sha1err.toStdString() << std::endl;

    sha1process.close();

    QProcess fileRenameProcess;
    QString attachmentSha1Filename = QString("/tmp/").append(attachmentSha1).append(".warp2.attachment");  //this is sloppy, need to find directory of original file dynamically
    QString renamePipe = "mv ";
    renamePipe.append(attachmentFileNameAsc);
    renamePipe.append(" ");
    renamePipe.append(attachmentSha1Filename);

//    std::cout << headerTempName.toStdString() << " " << headerSha1Filename.toStdString() << std::endl;

    fileRenameProcess.start("/bin/bash", QStringList() << "-c" << renamePipe);

    fileRenameProcess.setProcessChannelMode(QProcess::ForwardedChannels);
    fileRenameProcess.waitForFinished();

//    QString renameOutput = fileRenameProcess.readAllStandardOutput();
    QString renameError = fileRenameProcess.readAllStandardError();

    if(renameError.size() > 0)
    {
        QMessageBox::critical(
              this,
              "Critical Error",
              renameError);
    }
    else
    {
        attachmentFileName = attachmentSha1Filename;
        sendProgress->setAssembleFilesProgress(100);

    }
//    std::cout << "Rename Output:" << std::endl << renameOutput.toStdString() << std::endl;
//    std::cout << "Rename Error:" << std::endl << renameError.toStdString() << std::endl;
}




void MessageEditor::updateHeaderProgress(int value)
{
    sendProgress->setUploadHeaderProgress(value);
}

void MessageEditor::updateMessageProgress(int value)
{
    sendProgress->setUploadMessageProgress(value);
}

void MessageEditor::updateAttachmentProgress(int value)
{
    sendProgress->setUploadAttachmentProgress(value);
}

void MessageEditor::updateTotalProgress(int value)
{
    sendProgress->setTotalProgress(value);
}
