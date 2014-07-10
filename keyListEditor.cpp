#include <QProcess>
#include <QTableWidget>
#include <QFileDialog>
#include "keyListEditor.h"
#include "idcreator.h"
#include "ui_keylisteditor.h"

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

    saveDialog = new QFileDialog(this);

    saveDialog->setDirectory(QDir::homePath());

    saveDialog->setNameFilter("*.warp2ID.asc");

    saveDialog->show();
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
