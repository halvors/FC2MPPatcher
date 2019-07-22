#include "widget.h"
#include "ui_widget.h"

#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>

#include <QNetworkInterface>
#include <QHostAddress>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    loadSettings();

    setWindowTitle(Constants::app_name + " " + Constants::app_version);

    ui->lineEdit->setText(install_dir);
    populateComboboxWithNetworkInterfaces();

    patcher = new FC2MPPatcher(this);

    connect(ui->pushButton_dir, &QPushButton::clicked, this, &Widget::pushButton_dir_clicked);
    connect(ui->pushButton_patch, &QPushButton::clicked, this, &Widget::pushButton_patch_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::loadSettings()
{
    QSettings settings;
    install_dir = settings.value("install_dir", Constants::defualt_install_dir).toString();
}

void Widget::saveSettings()
{
    QSettings settings;
    settings.setValue("install_dir", install_dir);
}

void Widget::populateComboboxWithNetworkInterfaces()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();

    for (QNetworkInterface interface : list) {
        // Only show active network interfaces.
        if (interface.flags().testFlag(QNetworkInterface::IsUp)) {
            QString address;

            // Scan thru addresses for this interface.
            for (QNetworkAddressEntry addressEntry : interface.addressEntries()) {
                QHostAddress hostAddress = addressEntry.ip();

                // Only select IPv4 addresses.
                if (hostAddress.protocol() == QAbstractSocket::IPv4Protocol) {
                    address = hostAddress.toString();
                    break;
                }
            }

            ui->comboBox->addItem(interface.name() + " (" + address + ")");
        }
    }
}

void Widget::pushButton_dir_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this);

    install_dir = fileName;
    ui->lineEdit->setText(fileName);

    // TODO: Do this somewhere else.
    saveSettings();
}

void Widget::pushButton_patch_clicked()
{
    ui->pushButton_patch->setEnabled(false);
    ui->pushButton_patch->setText("Patching your broken yet awesome game...");

    // Create path to binary folder.
    QString path = install_dir + "/bin/";
    QString fileName = path + "FarCry2_patched.exe";

    // Copy original file to other workfile.
    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }

    QFile::copy(path + "FarCry2.exe", fileName);

    // Load the file into this program.
    patcher->open(fileName);

    patcher->addImportFunction(Constants::library_name, Constants::library_function_getAdaptersInfo);
    patcher->addImportFunction(Constants::library_name, Constants::library_function_getHostbyname);
    patcher->dumpImportDirectory();

    //peFile->peHeader().writeSections((filename).toStdString());
    //ui->pushButton_2->setText("Your game is now fixed! Enjoy the nostalgia of playing...");
}
