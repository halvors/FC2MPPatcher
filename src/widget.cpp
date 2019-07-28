#include <QNetworkInterface>
#include <QFileDialog>
#include <QDir>
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

    ui->label_installation_directory->setText("Select the " + Constants::game_name + " installation directory:");
    populateComboboxWithNetworkInterfaces();
    populateComboboxWithTargets();

    // Load settings from configuration file.
    settings = new QSettings(Constants::app_configuration_file, QSettings::IniFormat, this);
    loadSettings();

    connect(ui->pushButton_install_directory, &QPushButton::clicked, this, &Widget::pushButton_install_directory_clicked);
    connect(ui->pushButton_reset, &QPushButton::clicked, this, &Widget::pushButton_reset_clicked);
    connect(ui->pushButton_patch, &QPushButton::clicked, this, &Widget::pushButton_patch_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::loadSettings()
{
    QString install_directory = settings->value(Constants::settings_install_directory, Constants::game_install_directory).toString();
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
    QString installDirectory = QFileDialog::getExistingDirectory(this, "Select the " + Constants::game_name + " installation directory", ui->lineEdit_install_directory->text(), QFileDialog::ReadOnly);

    if (QDir(installDirectory).exists()) {
        ui->lineEdit_install_directory->setText(installDirectory);
    }

    saveSettings();
}

void Widget::pushButton_reset_clicked()
{
    QString path = ui->lineEdit_install_directory->text() + "/" + Constants::game_executable_directory + "/";
    TargetEntry target = ui->comboBox_select_target->currentData().value<TargetEntry>();

    QString fileName = path + target.fileName;
    QStringList split = fileName.split(".");
    QString fileNameBackup = split[0] + Constants::target_backup_suffix + split[1];

    if (QFile::exists(fileNameBackup)) {
        // Removed old main file.
        QFile::remove(fileName);

        // Copy backup to main file.
        QFile::copy(fileNameBackup, fileName);
    }
}

void Widget::pushButton_patch_clicked()
{
    // Create path to binary folder.
    QString path = ui->lineEdit_install_directory->text() + "/" + Constants::game_executable_directory + "/";
    TargetEntry target = ui->comboBox_select_target->currentData().value<TargetEntry>();

    // Verify that binary folder actually exists.
    if (!QDir(path).exists()) {
        QMessageBox::warning(this, "Warning", "Game file not found, please select the " + Constants::game_name + " installation directory.");
        return;
    }

    Patcher::generateNetworkConfigFile(path, ui->comboBox_network_interface->currentData().value<QNetworkAddressEntry>());

    // Validate target file against stored checksum.
    if (!Patcher::isFileValid(path, target)) {
        QMessageBox::warning(this, "Warning", "Invalid file checksum for " + target.fileName + ", patching was aborted!");

        return;
    }

    Patcher::applyPatch(path, ui->comboBox_select_target->currentData().value<TargetEntry>());
}
