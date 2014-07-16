#include <QProcess>
#include <QTableWidget>
#include <QFileDialog>
#include "keyListEditor.h"
#include "idcreator.h"
#include "ui_keylisteditor.h"
#include <QStandardPaths>
#include <QAbstractItemView>
#include <QMessageBox>

#include <iostream>

KeyListEditor::KeyListEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyListEditor)
{
    ui->setupUi(this);
    ui->privateKeyTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //Disable editing of key-rows
    ui->publicKeyTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //Disable editing of key-rows

    //find gpg2/gpg path
    gpgPath = QStandardPaths::findExecutable("gpg2", QStringList() << "/usr/local/bin/" << "/usr/bin/" << "/bin/" << "/usr/local/MacGPG2/bin/"); //H
    if(gpgPath.isEmpty()){
        //std::cout << "gpg2 not found" << std::endl;
        gpgPath = QStandardPaths::findExecutable("gpg", QStringList() << "/usr/local/bin/" << "/usr/bin/" << "/bin/");
    }
    if(gpgPath.isEmpty()){
        std::cerr << "gpg not found" << std::endl;
    }

    //std::cout << "Getting keys" << std::endl;
    loadPublicKeys();
    loadPrivateKeys();
}

void KeyListEditor::loadPublicKeys(){
    // GET THE PUBLIC KEYS OF ALL KNOWN CONTACTS
    int i;
    QProcess gpgGetKeys;

    //gpgGetKeys.start("gpg2  --list-keys");
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
            ui->publicKeyTable->setRowCount(addresses.size());
            ui->publicKeyTable->setColumnCount(1);
            ui->publicKeyTable->setItem(i,0, new QTableWidgetItem(addresses.at(i)));
            ui->publicKeyTable->setHorizontalHeaderLabels(QString("Contact;Username").split(";"));
        }
}

void KeyListEditor::loadPrivateKeys(){
    // GET MY PRIVATE KEY
    int i;
    QProcess gpgGetSecretKeys;

    //gpgGetSecretKeys.start("gpg2  --list-secret-keys");
    gpgGetSecretKeys.start(gpgPath, QStringList() << "--list-secret-keys");

    gpgGetSecretKeys.setProcessChannelMode(QProcess::ForwardedChannels);

    gpgGetSecretKeys.waitForFinished();

    QString output = gpgGetSecretKeys.readAllStandardOutput();

    QStringList linesplit = output.split(QRegExp("\>|\<"));
    QStringList fromaddresses = linesplit.filter(QRegExp(".*@.*"));

    fromaddresses.removeDuplicates();

    for(i=0; i<fromaddresses.size(); i++)
        {
            //linesplitTrimmed.append(potentialKeyPairs.at(i).simplified());
            //std::cout << fromaddresses.at(i).toStdString() << std::endl;
            ui->privateKeyTable->setRowCount(fromaddresses.size());
            ui->privateKeyTable->setColumnCount(1);
            ui->privateKeyTable->setItem(i,0,new QTableWidgetItem(fromaddresses.at(i)));
            ui->privateKeyTable->setHorizontalHeaderLabels(QString("ID;Username").split(";"));
        }
}
void KeyListEditor::updateKeys(){
    //std::cout << "updating" << std::endl;
    loadPrivateKeys();
    loadPublicKeys();
}

KeyListEditor::~KeyListEditor()
{
    delete ui;
}


void KeyListEditor::on_exportPublicKeyButton_clicked()
{
    QFileDialog * saveDialog;
    //Check if key is selected. If not, display info message.
    if(ui->privateKeyTable->selectedItems().isEmpty()){
        QMessageBox::information(
              this,
              "Action required",
              "Select key for exporting");
    }else{
        QString idStem = ui->privateKeyTable->selectedItems().first()->text();

        QString idName = idStem;

        idName.append(".warp2ID.asc");

        QString startpath;

        startpath = QDir::homePath();


        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                startpath,
                                tr("*.warp2ID.asc"));

        //If fileName is empty, user cancelled export
        if(!fileName.isEmpty()){

            //Remove any file extension that is given by trimming everything after the first "."

            QString finalFilename = fileName.section(".",0,0);

            finalFilename.append(".warp2ID.asc");

            std::cout << "Saving " << idStem.toStdString() << " as " << finalFilename.toStdString() << std::endl;

            std::cout << "Launching gpg process" << std::endl;

            // QString gpgScript = "--export --armor --output ";
            // gpgScript.append(finalFilename);
            // gpgScript.append(" ");
            // gpgScript.append(idStem);

            gpg.start(gpgPath, QStringList() << "--export" << "--armor" << "--output" << finalFilename << idStem);

            gpg.setProcessChannelMode(QProcess::ForwardedChannels);

            std::cout << "Connecting std out" << std::endl;
            connect(&gpg, SIGNAL(readyReadStandardOutput()), this, SLOT(readSlot()) );
            connect(&gpg, SIGNAL(readyReadStandardError()), this, SLOT(errorSlot()) );
        }

    }

}

void KeyListEditor::on_doneButton_clicked()
{
    QWidget::close();
}

void KeyListEditor::on_newPrivateIDButton_clicked()
{
    IDCreator *idc = new IDCreator(this);
    connect(idc, SIGNAL(updateKeys()), this, SLOT(updateKeys())); //Hanna
    idc->show();
}

void KeyListEditor::on_importContactButton_clicked()
{
    QString startpath = QDir::homePath();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Load File"),
                                startpath,
                                tr("*.warp2ID.asc"));

    std::cout << "Importing contact " << fileName.toStdString() << std::endl;

   // QString gpgScript = "gpg2 --import ";
   // gpgScript.append(fileName);

    gpg.start(gpgPath, QStringList() << "--import" << fileName);

    gpg.setProcessChannelMode(QProcess::ForwardedChannels);

    std::cout << "Connecting std out" << std::endl;
    connect(&gpg, SIGNAL(readyReadStandardOutput()), this, SLOT(readSlot()) );
    connect(&gpg, SIGNAL(readyReadStandardError()), this, SLOT(errorSlot()) );

    updateKeys(); //update keys in view
}


void KeyListEditor::readSlot(void)
{
    QString readData = gpg.readAllStandardOutput();
    std::cout << readData.toStdString() << std::endl;
}


void KeyListEditor::errorSlot(void)
{
    QString readData = gpg.readAllStandardError();
    std::cerr << readData.toStdString() << std::endl;
}
