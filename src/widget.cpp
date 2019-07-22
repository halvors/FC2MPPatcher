#include "widget.h"
#include "ui_widget.h"

#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QNetworkInterface>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle(Constants::app_name + " " + Constants::app_version);
    populateComboboxWithNetworkInterfaces();

    // Load settings from configuration file.
    settings = new QSettings(Constants::app_configuration_file, QSettings::IniFormat, this);
    loadSettings();

    patcher = new FC2MPPatcher(this);

    connect(ui->pushButton_install_dir, &QPushButton::clicked, this, &Widget::pushButton_install_dir_clicked);
    connect(ui->comboBox_network_interface, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &Widget::comboBox_network_interface_currentIndexChanged);
    connect(ui->pushButton_patch, &QPushButton::clicked, this, &Widget::pushButton_patch_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::loadSettings()
{
    resize(settings->value("size", size()).toSize());
    move(settings->value("position", pos()).toPoint());

    if (settings->value("isMaximized", false).toBool()) {
        showMaximized();
    }

    install_directory = settings->value(Constants::settings_install_directory, Constants::install_directory_list.first()).toString();
    ui->lineEdit_install_dir->setText(install_directory);

    interfaceIndex = settings->value(Constants::settings_interface_index, ui->comboBox_network_interface->currentIndex()).toInt();
    ui->comboBox_network_interface->setCurrentIndex(interfaceIndex);
}

void Widget::saveSettings()
{
    settings->setValue("size", size());
    settings->setValue("position", pos());
    settings->setValue("isMaximized", isMaximized());

    settings->setValue(Constants::settings_install_directory, install_directory);
    settings->setValue(Constants::settings_interface_index, ui->comboBox_network_interface->currentIndex());
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
    QString filename = install_directory + "/bin/" + Constants::network_configuration_file;
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
        install_directory = fileName;
        ui->lineEdit_install_dir->setText(fileName);

        // TODO: Do this somewhere else.
        saveSettings();
    }
}

void Widget::comboBox_network_interface_currentIndexChanged(int index)
{
    interfaceIndex = index;
}

void Widget::pushButton_patch_clicked()
{
    ui->pushButton_patch->setEnabled(false);
    ui->pushButton_patch->setText("Patching your broken yet awesome game...");

    patcher->patch(install_directory);

    //ui->pushButton_2->setText("Your game is now fixed! Enjoy the nostalgia of playing...");

    generateNetworkConfigFile(ui->comboBox_network_interface->currentData().toString());

    // TODO: Do this somewhere else.
    saveSettings();
}
