#include <QDir>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QFileDialog>

#include "widget.h"
#include "ui_widget.h"
#include "fileutils.h"
#include "patcher.h"

#include "dirutils.h"

Widget::Widget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle(Constants::app_name + " " + Constants::app_version);

    // Load settings from configuration file.
    settings = new QSettings(Constants::app_configuration_file, QSettings::IniFormat, this);
    loadSettings();

    // Setup UI.
    ui->label_installation_directory->setText("Select the " + Constants::game_name + " installation directory:");
    populateComboboxWithNetworkInterfaces();

    bool patched = Patcher::isPatched(getInstallDir(false));
    updatePatchStatus(patched);

    connect(ui->pushButton_install_directory, &QPushButton::clicked, this, &Widget::pushButton_install_directory_clicked);
    //connect(ui->lineEdit_install_directory, &QLineEdit::textChanged, this, &Widget::updateInstallDir);
    connect(ui->comboBox_network_interface, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::comboBox_network_interface_currentIndexChanged);
    connect(ui->pushButton_patch, &QPushButton::clicked, this, &Widget::pushButton_patch_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent* event)
{
    saveSettings();

    QWidget::closeEvent(event);
}

void Widget::loadSettings()
{
    QString installDir = settings->value(Constants::settings_install_directory, findInstallDir().absolutePath()).toString();
    ui->lineEdit_install_directory->setText(installDir);

    int index = settings->value(Constants::settings_interface_index).toInt();

    // Only set valid index in UI.
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

void Widget::saveSettings() const
{
    QString installDir = ui->lineEdit_install_directory->text();

    if (!installDir.isEmpty()) {
        settings->setValue(Constants::settings_install_directory, installDir);
    }

    settings->setValue(Constants::settings_interface_index, ui->comboBox_network_interface->currentIndex());

    settings->beginGroup(Constants::settings_group_window);
        settings->setValue(Constants::settings_group_window_size, size());
        settings->setValue(Constants::settings_group_window_position, pos());
        settings->setValue(Constants::settings_group_window_isMaximized, isMaximized());
    settings->endGroup();
}

QDir Widget::findInstallDir() const
{
    // Look for Far Cry 2 install directory in registry.
    QDir dir = DirUtils::getRetailInstallDir();

    if (DirUtils::isGameDir(dir)) {
        return dir;
    }

    // Look for Far Cry 2 install directory in Steam.
    dir = DirUtils::getSteamInstallDir(Constants::game_steam_appId);

    if (DirUtils::isGameDir(dir)) {
        return dir;
    }

    // Fallback to statically set installation directory if autodetection failed.
    return Constants::game_install_directory;
}

QDir Widget::getInstallDir(bool showWarning)
{
    // Create path to binary folder.
    QDir dir = ui->lineEdit_install_directory->text();

    if (DirUtils::isGameDir(dir)) {
        return dir;
    }

    if (showWarning) {
        QMessageBox::warning(this, "Warning", Constants::game_name + " installation directory not found, please select it manually.");
    }

    return QDir();
}

void Widget::updateInstallDir(QDir dir)
{
    if (DirUtils::isGameDir(dir)) {
        ui->lineEdit_install_directory->setText(dir.absolutePath());
    }
}

void Widget::populateComboboxWithNetworkInterfaces() const
{
    // Loop thru all of the systems network interfaces.
    for (const QNetworkInterface &interface : QNetworkInterface::allInterfaces()) {
        const QNetworkInterface::InterfaceFlags &flags = interface.flags();

        // Only show active network interfaces.
        if (flags.testFlag(QNetworkInterface::IsUp) && !flags.testFlag(QNetworkInterface::IsLoopBack)) {
            QNetworkAddressEntry selectedAddressEntry;

            // Scan thru addresses for this interface.
            for (const QNetworkAddressEntry &addressEntry : interface.addressEntries()) {
                const QHostAddress &hostAddress = addressEntry.ip();
                // Only select first IPv4 address found.
                if (hostAddress.protocol() == QAbstractSocket::IPv4Protocol) {
                    selectedAddressEntry = addressEntry;
                    break;
                }
            }

            // TODO: Handle this if it's empty.
            ui->comboBox_network_interface->addItem(interface.name() + " (" + selectedAddressEntry.ip().toString() + ")", QVariant::fromValue<QNetworkAddressEntry>(selectedAddressEntry));
        }
    }
}

void Widget::updatePatchStatus(bool patched) const
{
    ui->pushButton_patch->setText(!patched ? "Patch" : "Un-patch");
}

void Widget::pushButton_install_directory_clicked()
{
    QDir dir = QFileDialog::getExistingDirectory(this, "Select the " + Constants::game_name + " installation directory", ui->lineEdit_install_directory->text(), QFileDialog::ReadOnly);

    updateInstallDir(dir);
}

void Widget::comboBox_network_interface_currentIndexChanged(int index)
{
    // Generate network configuration.
    Patcher::generateNetworkConfigFile(getInstallDir(), ui->comboBox_network_interface->itemData(index).value<QNetworkAddressEntry>());
}

void Widget::pushButton_patch_clicked()
{
    // Create path to binary folder.
    QDir path = getInstallDir();

    // Only show option to patch if not already patched.
    if (Patcher::isPatched(path)) {
        Patcher::undoPatch(path);

        updatePatchStatus(false);
    } else {
        // Apply patch to files, if successful continue.
        if (Patcher::patch(this, path)) {
            // Generate network configuration.
            Patcher::generateNetworkConfigFile(path, ui->comboBox_network_interface->currentData().value<QNetworkAddressEntry>());

            updatePatchStatus(true);
        }
    }
}
