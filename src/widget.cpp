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

    setWindowTitle(Constants::app_name + " " + Constants::app_version);
    populateComboboxWithNetworkInterfaces();

    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, Constants::app_name, Constants::app_name, this);
    loadSettings();

    patcher = new FC2MPPatcher(this);

    connect(ui->pushButton_install_dir, &QPushButton::clicked, this, &Widget::pushButton_install_dir_clicked);
    connect(ui->pushButton_patch, &QPushButton::clicked, this, &Widget::pushButton_patch_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::loadSettings()
{
    install_dir = settings->value("install_dir", Constants::defualt_install_dir).toString();
    ui->lineEdit_install_dir->setText(install_dir);

    interfaceIndex = settings->value("interface_index", ui->comboBox_network_interface->currentIndex()).toInt();
    ui->comboBox_network_interface->setCurrentIndex(interfaceIndex);
}

void Widget::saveSettings()
{
    settings->setValue("install_dir", install_dir);
    settings->setValue("interface_index", ui->comboBox_network_interface->currentIndex());
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

            ui->comboBox_network_interface->addItem(interface.name() + " (" + address + ")", address);
        }
    }
}

bool Widget::generateNetworkConfigFile(const QString &address)
{
    QString filename = install_dir + "/bin/" + Constants::network_configuration_file;
    QFile file(filename);

    // TODO: Handle if new input is smaller than existing file contents.

    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << address << endl;

        return true;
    }

    return false;
}

void Widget::pushButton_install_dir_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this);

    if (!fileName.isEmpty()) {
        install_dir = fileName;
        ui->lineEdit_install_dir->setText(fileName);

        // TODO: Do this somewhere else.
        saveSettings();
    }
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
    //patcher->addImportFunction(Constants::library_name, Constants::library_function_getHostbyname);

    patcher->dumpImportDirectory();

    //peFile->peHeader().writeSections((filename).toStdString());
    //ui->pushButton_2->setText("Your game is now fixed! Enjoy the nostalgia of playing...");

    generateNetworkConfigFile(ui->comboBox_network_interface->currentData().toString());

    // TODO: Do this somewhere else.
    saveSettings();
}
