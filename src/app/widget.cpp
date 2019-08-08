#include <QDir>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QFileDialog>

#include <QVersionNumber>

#include "widget.h"
#include "ui_widget.h"
#include "dirutils.h"
#include "fileutils.h"
#include "patcher.h"

Widget::Widget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle(QString("%1 %2").arg(Constants::app_name, QVersionNumber(Constants::app_version_major, Constants::app_version_minor, Constants::app_version_micro).toString()));

    // Load settings from configuration file.
    settings = new QSettings(Constants::app_configuration_file, QSettings::IniFormat, this);
    loadSettings();

    // Setup UI.
    ui->label_installation_directory->setText(QString("Select the %1 installation directory:").arg(Constants::game_name));
    populateComboboxWithNetworkInterfaces();

    bool patched = Patcher::isPatched(getInstallDir(false));
    updatePatchStatus(patched);

    connect(ui->pushButton_install_directory, &QPushButton::clicked, this, &Widget::pushButton_install_directory_clicked);
    //connect(ui->lineEdit_install_directory, &QLineEdit::editingFinished, this, &Widget::lineEdit_install_directory_editingFinished);
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
    QString installDir = settings->value(Constants::settings_install_directory).toString();

    if (!DirUtils::isGameDir(installDir)) {
        installDir = DirUtils::findInstallDir();

        qDebug() << "Detected install directory:" << installDir;
    }

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
    settings->setValue(Constants::settings_install_directory, installDir);
    settings->setValue(Constants::settings_interface_index, ui->comboBox_network_interface->currentIndex());

    settings->beginGroup(Constants::settings_group_window);
        settings->setValue(Constants::settings_group_window_size, size());
        settings->setValue(Constants::settings_group_window_position, pos());
        settings->setValue(Constants::settings_group_window_isMaximized, isMaximized());
    settings->endGroup();
}

QString Widget::getInstallDir(bool showWarning)
{
    // Create path to binary folder.
    QDir dir = ui->lineEdit_install_directory->text();

    if (DirUtils::isGameDir(dir)) {
        return dir.absolutePath();
    }

    if (showWarning) {
        QMessageBox::warning(this, "Warning", QString("%1 installation directory not found, please select it manually.").arg(Constants::game_name ));
    }

    return QString();
}

void Widget::updateInstallDir(const QString &installDir)
{
    if (DirUtils::isGameDir(installDir)) {
        ui->lineEdit_install_directory->setText(installDir);
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
    QString installDir = QFileDialog::getExistingDirectory(this, QString("Select the %1 installation directory").arg(Constants::game_name), ui->lineEdit_install_directory->text(), QFileDialog::ReadOnly);

    updateInstallDir(installDir);
}

void Widget::comboBox_network_interface_currentIndexChanged(int index)
{
    QString installDir = getInstallDir();

    if (!installDir.isNull()) {
        // Generate network configuration.
        Patcher::generateNetworkConfigFile(installDir, ui->comboBox_network_interface->itemData(index).value<QNetworkAddressEntry>());
    }
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
