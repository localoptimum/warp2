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
