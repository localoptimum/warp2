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


#include <iostream>
#include <QProcess>
#include "idcreator.h"
#include "ui_idcreator.h"
#include "keygenerator.h"

IDCreator::IDCreator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IDCreator)
{
    ui->setupUi(this);
}

IDCreator::~IDCreator()
{
    delete ui;
}

void IDCreator::on_IDgenerateOK_clicked()
{
    //Is username sensible?
    QString rawUsername = ui->enteredUsername->text();

    if(rawUsername == QString::null || rawUsername == "")
    {
        std::cout << "No username entered" << std::endl;
        return;
    }

    //Strip away everything following the @ symbol if they put in an email address

    QString fullusername = rawUsername.split("@").at(0);

    QString simpleusername = fullusername;

    //Add our own non-existent inbox address as a tag
    fullusername.append("@warp2.net");


    //Is password entered?  This can be anything that's not null

    QString password = ui->enteredPassword->text();

    //Is password good?

    if(password == QString::null || password == "")
    {
        std::cout << "No password entered" << std:: endl;
        return;
    }

    //If we get here, everything is ready to launch gpg --gen-key --batch...

    std::cout << "Generating keys..." << std::endl;
    std::cout << fullusername.toStdString() << std::endl;
    std::cout << password.toStdString() << std::endl;

    keyGenerator * keygen = new keyGenerator();
    connect(keygen, SIGNAL(keyGenerated()), this, SLOT(keyGenerated())); //connect keygenerator to keylisteditor, for updating

    int keygenerror = keygen->createKeys(fullusername, simpleusername, password);

    //check response here, errors etc
    if(keygenerror == 0)
    {
        std::cout << "Key generation successful" << std::endl;
    }
    else
    {
        std::cerr << "Error generating keys" << std::endl;
    }

    QWidget::close();
}

void IDCreator::on_IDgenerateCancel_clicked()
{
    QWidget::close();
}

void IDCreator::keyGenerated(){
    emit updateKeys();
}
