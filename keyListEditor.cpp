#include <QProcess>
#include <QTableWidget>
#include <QFileDialog>
#include "keyListEditor.h"
#include "idcreator.h"
#include "ui_keylisteditor.h"

#include <iostream>

KeyListEditor::KeyListEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyListEditor)
{
    int i;
    ui->setupUi(this);

    //std::cout << "Getting keys" << std::endl;

    QProcess gpgGetKeys;
    QProcess gpgGetSecretKeys;



    // GET THE PUBLIC KEYS OF ALL KNOWN CONTACTS

    gpgGetKeys.start("gpg2  --list-keys");

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



    // GET MY PRIVATE KEY

    gpgGetSecretKeys.start("gpg2 --list-secret-keys");

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
            ui->privateKeyTable->setRowCount(fromaddresses.size());
            ui->privateKeyTable->setColumnCount(1);
            ui->privateKeyTable->setItem(i,0,new QTableWidgetItem(fromaddresses.at(i)));
            ui->privateKeyTable->setHorizontalHeaderLabels(QString("ID;Username").split(";"));
        }
}

KeyListEditor::~KeyListEditor()
{
    delete ui;
}


void KeyListEditor::on_exportPublicKeyButton_clicked()
{
    QFileDialog * saveDialog;

    QString idStem = ui->privateKeyTable->selectedItems().first()->text();

    QString idName = idStem;

    idName.append(".warp2ID.asc");

    QString startpath;

    startpath = QDir::homePath();


    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                startpath,
                                tr("*.warp2ID.asc"));

    //Remove any file extension that is given by trimming everything after the first "."

    QString finalFilename = fileName.section(".",0,0);

    finalFilename.append(".warp2ID.asc");

    std::cout << "Saving " << idStem.toStdString() << " as " << finalFilename.toStdString() << std::endl;

    std::cout << "Launching gpg process" << std::endl;

    QString gpgScript = "gpg2 --export --armor --output ";
    gpgScript.append(finalFilename);
    gpgScript.append(" ");
    gpgScript.append(idStem);

    gpg.start("/bin/bash", QStringList() << "-c" << gpgScript);

    gpg.setProcessChannelMode(QProcess::ForwardedChannels);

    std::cout << "Connecting std out" << std::endl;
    connect(&gpg, SIGNAL(readyReadStandardOutput()), this, SLOT(readSlot()) );
    connect(&gpg, SIGNAL(readyReadStandardError()), this, SLOT(errorSlot()) );



}

void KeyListEditor::on_doneButton_clicked()
{
    QWidget::close();
}

void KeyListEditor::on_newPrivateIDButton_clicked()
{
    IDCreator *idc = new IDCreator(this);
    idc->show();
}

void KeyListEditor::on_importContactButton_clicked()
{
    QFileDialog *loadDialog;

    loadDialog = new QFileDialog(this);

    loadDialog->setDirectory(QDir::homePath());

    loadDialog->setNameFilter("*.warp2ID.asc");

    loadDialog->show();
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
