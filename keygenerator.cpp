#include "keygenerator.h"
#include <QString>
#include <QProcess>
#include <iostream>

keyGenerator::keyGenerator(QObject *parent) :
    QObject(parent)
{
}


int keyGenerator::createKeys(QString username, QString simpleusername, QString password)
{

    std::cout << "Launching gpg process" << std::endl;

    QString gpgScript = "gpg2 --gen-key --batch --no-tty";

    gpg.start("/bin/bash", QStringList() << "-c" << gpgScript);

    gpg.setProcessChannelMode(QProcess::ForwardedChannels);

    std::cout << "Connecting std out" << std::endl;
    connect(&gpg, SIGNAL(readyReadStandardOutput()), this, SLOT(readSlot()) );
    connect(&gpg, SIGNAL(readyReadStandardError()), this, SLOT(errorSlot()) );


    gpg.write("%echo Generating a warp2 keypair\n");
    gpg.write("Key-Type: RSA\n");
    gpg.write("Key-Length: 2048\n");
    gpg.write("Subkey-Type: RSA\n");
    gpg.write("Subkey-Length: 2048\n");
    gpg.write("Name-Real: ");
    gpg.write(simpleusername.toStdString().c_str());
    gpg.write("\n");
    gpg.write("Name-Comment: warp2\n");
    gpg.write("Name-Email: ");
    gpg.write(username.toStdString().c_str());
    gpg.write("\n");
    gpg.write("Expire-Date: 0\n");
    gpg.write("Passphrase: ");
    gpg.write(password.toStdString().c_str());
    gpg.write("\n");
    //gpg.write("%pubring warp2.pub\n");
    //gpg.write("%secring warp2.sec\n");
    gpg.write("%commit\n");
    gpg.write("%echo warp2 keypair done\n");

    gpg.waitForBytesWritten();


    //gpg.waitForReadyRead();

        std::cout << "reading gpg process" << std::endl;


//    QString output(gpg.readAllStandardOutput());
//    QString errorout(gpg.readAllStandardError());

//    std::cout << output.toStdString() << std::endl;
//    std::cout << errorout.toStdString() << std::endl;

    return(0);
}

    void keyGenerator::readSlot(void)
    {
        QString readData = gpg.readAllStandardOutput();
        std::cout << readData.toStdString() << std::endl;
    }


    void keyGenerator::errorSlot(void)
    {
        QString readData = gpg.readAllStandardError();
        std::cerr << readData.toStdString() << std::endl;
    }
