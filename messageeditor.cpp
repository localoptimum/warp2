#include "messageeditor.h"
#include "ui_messageeditor.h"

#include <QFileDialog>
#include <QProcess>
#include <QDateTime>
#include <QCryptographicHash>
#include <QTemporaryFile>
#include <iostream>

MessageEditor::MessageEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageEditor)
{
    int i;

    ui->setupUi(this);

    //std::cout << "Getting keys" << std::endl;

    QProcess gpgGetKeys;
    QProcess gpgGetSecretKeys;



    // GET THE PUBLIC KEYS OF ALL KNOWN CONTACTS

    gpgGetKeys.start("gpg  --list-keys");

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

    gpgGetSecretKeys.start("gpg  --list-secret-keys");

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
    //Assemble header material into header string

    QString msgHeader;

    msgHeader = "Warp2Header;Version:Beta;";
    msgHeader.append("From:");
    msgHeader.append(ui->fromPullDown->currentText());
    msgHeader.append(";");

    msgHeader.append("To:");
    msgHeader.append(ui->addresseePullDown->currentText());
    msgHeader.append(";");

    QDateTime rightnow(QDateTime::currentDateTime());
    msgHeader.append(rightnow.toString());
    msgHeader.append(";");

    msgHeader.append("Subject:");
    msgHeader.append(ui->subjectTextBox->text());
    msgHeader.append(";");

    std::cout << msgHeader.toStdString() << std::endl;



    //Make tempfile of header

    QTemporaryFile headerTemp;

    headerTemp.open();
    headerTemp.setAutoRemove(false);  // we will manually remove the temp files after send process is finished

    QString headerTempName = headerTemp.fileName();

    headerTemp.close();



    //Encrypt header

    QProcess encryptProcess;
    QString encryptOutput;
    QString encryptError;

    QString encryptPipe = "gpg -e -u ";
    encryptPipe.append(ui->fromPullDown->currentText());
    encryptPipe.append(" -r ");
    encryptPipe.append(ui->addresseePullDown->currentText());
    encryptPipe.append(" >> ");
    encryptPipe.append(headerTempName);

    std::cout << encryptPipe.toStdString() << std::endl;

    encryptProcess.start("/bin/bash", QStringList() << "-c" << encryptPipe);

    encryptProcess.setProcessChannelMode(QProcess::ForwardedChannels);

    encryptProcess.write(msgHeader.toAscii()); // then wait for bytes written before reading

    encryptProcess.waitForBytesWritten();
    encryptProcess.closeWriteChannel();
    encryptProcess.waitForFinished();

    encryptOutput = encryptProcess.readAllStandardOutput();
    encryptError = encryptProcess.readAllStandardError();

    std::cout << "Output:" << std::endl << encryptOutput.toStdString() << std::endl;
    std::cout << "Error:" << std::endl << encryptError.toStdString() << std::endl;

    encryptProcess.close();

//    //Make tempfile of header

//    QTemporaryFile headerTemp;

//    headerTemp.open();
//    headerTemp.setAutoRemove(false);  // we will manually remove the temp files after send process is finished


//    headerTemp.write(msgHeader.toLocal8Bit());



//    QProcess encryptProcess;


//    //Sha1 header

//    QProcess sha1process;

//    sha1process.start("sha1sum", QStringList() << headerTempName);

//    sha1process.setProcessChannelMode(QProcess::ForwardedChannels);

//    sha1process.waitForFinished();

//    QString headerSha1(sha1process.readAllStandardOutput());
//    QString sha1err(sha1process.readAllStandardError());

//    sha1process.close();

//    std::cout << headerTempName.toStdString() << std::endl;

//    std::cout << headerSha1.toStdString() << std::endl;
//    //std::cout << sha1err.toStdString() << std::endl;



//    //Sha1 body

//    QString messageBodyText = ui->messageTextBox->toPlainText();
//    QTemporaryFile messageTemp;

//    messageTemp.open();

//    QString messageTempFileName = messageTemp.fileName();

//    messageTemp.write(messageBodyText.toLocal8Bit());
//    messageTemp.setAutoRemove(false);  // we will manually remove the temp files

//    messageTemp.close();


//    encryptProcess.start("gpg --")


    //Encrypt message body
    //Make tempfile of body

    //Sha1 attachment
    //Encrypt attachment
    //Make tempfile of attachment


    //Compute timestamp token
    //Upload header to server

    //Compute timestamp token
    //Upload message to server

    //Compute timestamp token
    //Upload attachment to server

    std::cout << "Message sent" << std::endl;

    QWidget::close();
}


