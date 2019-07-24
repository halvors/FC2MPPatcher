#include "widget.h"
#include "ui_widget.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QNetworkInterface>
#include <QDebug>

#include <QTimer>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    patcher(new Patcher(this))
{
    ui->setupUi(this);

    setWindowTitle(Constants::app_name + " " + Constants::app_version);
    populateComboboxWithNetworkInterfaces();

    // Load settings from configuration file.
    settings = new QSettings(Constants::app_configuration_file, QSettings::IniFormat, this);
    loadSettings();

    connect(ui->pushButton_install_directory, &QPushButton::clicked, this, &Widget::pushButton_install_directory_clicked);
    connect(ui->pushButton_patch, &QPushButton::clicked, this, &Widget::pushButton_patch_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::loadSettings()
{
    QString install_directory = settings->value(Constants::settings_install_directory, Constants::install_directory_list.first()).toString();
    ui->lineEdit_install_directory->setText(install_directory);

    int index = settings->value(Constants::settings_interface_index).toInt();

    if (index < ui->comboBox_network_interface->count()) {
        ui->comboBox_network_interface->setCurrentIndex(index);
    }

    settings->beginGroup("Window");
        resize(settings->value("size", size()).toSize());
        move(settings->value("position", pos()).toPoint());

        if (settings->value("isMaximized", false).toBool()) {
            showMaximized();
        }
    settings->endGroup();
}

void Widget::saveSettings()
{
    settings->setValue(Constants::settings_install_directory, ui->lineEdit_install_directory->text());
    settings->setValue(Constants::settings_interface_index, ui->comboBox_network_interface->currentIndex());

    settings->beginGroup("Window");
        settings->setValue("size", size());
        settings->setValue("position", pos());
        settings->setValue("isMaximized", isMaximized());
    settings->endGroup();
}

void Widget::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);

    saveSettings();
}

void Widget::populateComboboxWithNetworkInterfaces()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();

    for (QNetworkInterface interface : list) {
        QNetworkInterface::InterfaceFlags flags = interface.flags();

        // Only show active network interfaces.
        if (flags.testFlag(QNetworkInterface::IsUp) && !flags.testFlag(QNetworkInterface::IsLoopBack)) {
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

bool Widget::generateNetworkConfigFile(const QString &installDirectory, const QString &address)
{
    QFile file(installDirectory + "/" + Constants::executable_directory + "/" + Constants::network_configuration_file);

    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream << address << endl;

        return true;
    }

    return false;
}

void Widget::pushButton_install_directory_clicked()
{
    QString installDirectory = QFileDialog::getExistingDirectory(this, nullptr, ui->lineEdit_install_directory->text());

    if (QDir(installDirectory).exists()) {
        ui->lineEdit_install_directory->setText(installDirectory);
    }
}

void Widget::pushButton_patch_clicked()
{
    // Create path to binary folder.
    QString path = ui->lineEdit_install_directory->text() + "/" + Constants::executable_directory;
    QString fileName = path + "/FarCry2_patched.exe";

    // Copy original file to other workfile.
    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }

    QFile::copy(path + "/FarCry2.exe", fileName);

    qDebug() << "FarCry2_patched.exe was reset.";

    patcher->applyPatch(ui->lineEdit_install_directory->text());
    generateNetworkConfigFile(ui->lineEdit_install_directory->text(), ui->comboBox_network_interface->currentData().toString());

    // Used to indicate how many times this button was pressed since application start.
    ui->pushButton_patch->setText(ui->pushButton_patch->text() + ".");

    //ui->pushButton_patch->setEnabled(false);
    //ui->pushButton_patch->setText("Patching your broken yet awesome game...");
    //ui->pushButton_patch->setText("Your game is now fixed! Enjoy the nostalgia of playing...");
}
