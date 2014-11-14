//Copyright (c) 2014, Phil Bentley
//All rights reserved.

//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:

//1. Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.

//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
//ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//The views and conclusions contained in the software and documentation are those
//of the authors and should not be interpreted as representing official policies,
//either expressed or implied, of the University of Uppsala or the European
//Spallation Source.

#include "keygenerator.h"
#include <QString>
#include <QProcess>
#include <iostream>
#include <QStandardPaths>

keyGenerator::keyGenerator(QObject *parent) :
    QObject(parent)
{
}


int keyGenerator::createKeys(QString username, QString simpleusername, QString password)
{

    std::cout << "Launching gpg process" << std::endl;
    //find gpg2/gpg path
    QString gpgPath = QStandardPaths::findExecutable("gpg2", QStringList() << "/usr/local/bin/" << "/usr/bin/" << "/bin/" << "/usr/local/MacGPG2/bin/");
    if(gpgPath.isEmpty()){
        //std::cout << "gpg2 not found" << std::endl;
        gpgPath = QStandardPaths::findExecutable("gpg", QStringList() << "/usr/local/bin/" << "/usr/bin/" << "/bin/");
    }
    if(gpgPath.isEmpty()){
        std::cerr << "gpg not found" << std::endl;
    }
    gpg.start(gpgPath, QStringList() << "--gen-key" << "--batch" << "--no-tty");

    //QString gpgScript = "gpg2 --gen-key --batch --no-tty";
    //gpg.start("/bin/bash", QStringList() << "-c" << gpgScript);

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

    void keyGenerator::readSlot(void) // all output is directed to stderr..
    {
        QString readData = gpg.readAllStandardOutput();
        std::cout << readData.toStdString() << std::endl;
    }


    void keyGenerator::errorSlot(void)
    {
        QString readData = gpg.readAllStandardError();
        std::cerr << readData.toStdString() << std::endl;
        emit keyGenerated(); //emitting three times per key generated
    }
