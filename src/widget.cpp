#include <QVariant>
#include <QNetworkInterface>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include "widget.h"
#include "ui_widget.h"
#include "patcher.h"

Widget::Widget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle(Constants::app_name + " " + Constants::app_version);
    populateComboboxWithNetworkInterfaces();
    populateComboboxWithTargets();

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
    QString install_directory = settings->value(Constants::settings_install_directory, Constants::install_directory).toString();
    ui->lineEdit_install_directory->setText(install_directory);

    int index = settings->value(Constants::settings_interface_index).toInt();

    if (index < ui->comboBox_network_interface->count()) {
        ui->comboBox_network_interface->setCurrentIndex(index);
    }

    settings->beginGroup(Constants::settings_group_window);
        resize(settings->value(Constants::settings_group_window_size, size()).toSize());
        move(settings->value(Constants::settings_group_window_position, pos()).toPoint());

        if (settings->value(Constants::settings_group_window_isMaximized, false).toBool()) {
            showMaximized();
        }
    settings->endGroup();
}

void Widget::saveSettings()
{
    settings->setValue(Constants::settings_install_directory, ui->lineEdit_install_directory->text());
    settings->setValue(Constants::settings_interface_index, ui->comboBox_network_interface->currentIndex());

    settings->beginGroup(Constants::settings_group_window);
        settings->setValue(Constants::settings_group_window_size, size());
        settings->setValue(Constants::settings_group_window_position, pos());
        settings->setValue(Constants::settings_group_window_isMaximized, isMaximized());
    settings->endGroup();
}

void Widget::closeEvent(QCloseEvent* event)
{
    QWidget::closeEvent(event);

    saveSettings();
}

void Widget::populateComboboxWithNetworkInterfaces()
{
    for (QNetworkInterface &interface : QNetworkInterface::allInterfaces()) {
        QNetworkInterface::InterfaceFlags flags = interface.flags();

        // Only show active network interfaces.
        if (flags.testFlag(QNetworkInterface::IsUp) && !flags.testFlag(QNetworkInterface::IsLoopBack)) {
            QNetworkAddressEntry selectedAddressEntry;

            // Scan thru addresses for this interface.
            for (QNetworkAddressEntry &addressEntry : interface.addressEntries()) {
                QHostAddress hostAddress = addressEntry.ip();
                // Only select IPv4 addresses.
                if (hostAddress.protocol() == QAbstractSocket::IPv4Protocol) {
                    selectedAddressEntry = addressEntry;
                    break;
                }
            }

            ui->comboBox_network_interface->addItem(interface.name() + " (" + selectedAddressEntry.ip().toString() + ")", QVariant::fromValue<QNetworkAddressEntry>(selectedAddressEntry));
        }
    }
}

void Widget::populateComboboxWithTargets()
{
    for (TargetEntry target : Constants::targets) {
        ui->comboBox_select_target->addItem(target.fileName, QVariant::fromValue<TargetEntry>(target));
    }
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
    QString path = ui->lineEdit_install_directory->text() + "/" + Constants::executable_directory + "/";
    TargetEntry target = ui->comboBox_select_target->currentData().value<TargetEntry>();

    // Validate target file against stored checksum.
    if (!Patcher::isValid(path, target)) {
        QMessageBox::critical(this, "Error", "Invalid file checksum for " + target.fileName + ", patching was aborted!");

        return;
    }

    Patcher::applyPatch(path, ui->comboBox_select_target->currentData().value<TargetEntry>());
    Patcher::generateNetworkConfigFile(path, ui->comboBox_network_interface->currentData().value<QNetworkAddressEntry>());
}
