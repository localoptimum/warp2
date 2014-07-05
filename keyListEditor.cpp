#include <QProcess>
#include <QTableWidget>
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
            ui->publicKeyTable->setRowCount(addresses.size());
            ui->publicKeyTable->setColumnCount(1);
            ui->publicKeyTable->setItem(i,0, new QTableWidgetItem(addresses.at(i)));
            ui->publicKeyTable->setHorizontalHeaderLabels(QString("Contact;Username").split(";"));
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

void KeyListEditor::on_toolButton_3_clicked()
{
    QWidget::close();
}

void KeyListEditor::on_toolButton_clicked()
{
    IDCreator *idc = new IDCreator(this);
    idc->show();
}
