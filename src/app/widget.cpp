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

    setWindowTitle(QString("%1 %2").arg(app_name, QVersionNumber(app_version_major, app_version_minor, app_version_micro).toString()));

    // Load settings from configuration file.
    settings = new QSettings(app_configuration_file, QSettings::IniFormat, this);
    loadSettings();

    // Setup UI.
    populateComboboxWithInstallDirectories();

    ui->label_installation_directory->setText(tr("Select the %1 installation directory:").arg(game_name));
    populateComboboxWithNetworkInterfaces();

    bool patched = Patcher::isPatched(getInstallDirectory(false));
    updatePatchStatus(patched);

    // Register GUI signals to slots.
    connect(ui->pushButton_install_directory,   &QPushButton::clicked,                                  this, &Widget::pushButton_install_directory_clicked);
    //connect(ui->lineEdit_install_directory,   &QLineEdit::editingFinished,                            this, &Widget::lineEdit_install_directory_editingFinished);
    connect(ui->comboBox_network_interface,     QOverload<int>::of(&QComboBox::currentIndexChanged),    this, &Widget::comboBox_network_interface_currentIndexChanged);
    connect(ui->pushButton_patch,               &QPushButton::clicked,                                  this, &Widget::pushButton_patch_clicked);

    // Register signals to saveSettings slot.
    connect(ui->comboBox_network_interface,     QOverload<int>::of(&QComboBox::currentIndexChanged),    this, &Widget::saveSettings);
    connect(ui->pushButton_patch,               &QPushButton::clicked,                                  this, &Widget::saveSettings);
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
    QString installDirectory = settings->value(settings_install_directory).toString();

    if (DirUtils::isGameDirectory(installDirectory)) {
        const QStringList &installDirectories = DirUtils::findInstallDirectories();

        if (!installDirectories.contains(installDirectory)) {
            ui->comboBox_install_directory->insertItem(0, installDirectory);
        }
    }

    int index = settings->value(settings_interface_index).toInt();

    // Only set valid index in UI.
    if (index < ui->comboBox_network_interface->count()) {
        ui->comboBox_network_interface->setCurrentIndex(index);
    }

    settings->beginGroup(settings_group_window);
        resize(settings->value(settings_group_window_size, size()).toSize());
        move(settings->value(settings_group_window_position, pos()).toPoint());

        if (settings->value(settings_group_window_isMaximized, false).toBool()) {
            showMaximized();
        }
    settings->endGroup();
}

void Widget::saveSettings() const
{
    QString installDirectory = ui->comboBox_install_directory->currentText();

    if (DirUtils::isGameDirectory(installDirectory)) {
        settings->setValue(settings_install_directory, installDirectory);
    }

    settings->setValue(settings_interface_index, ui->comboBox_network_interface->currentIndex());

    settings->beginGroup(settings_group_window);
        settings->setValue(settings_group_window_size, size());
        settings->setValue(settings_group_window_position, pos());
        settings->setValue(settings_group_window_isMaximized, isMaximized());
    settings->endGroup();
}

QString Widget::getInstallDirectory(bool warning)
{
    // Create path to binary folder.
    QDir dir = ui->comboBox_install_directory->currentText();

    if (DirUtils::isGameDirectory(dir)) {
        return dir.absolutePath();
    }

    if (warning) {
        QMessageBox::warning(this, "Warning", tr("%1 installation directory not found, please select it manually.").arg(game_name));
    }

    return QString();
}

void Widget::populateComboboxWithInstallDirectories() const
{
    QStringList installDirectories = DirUtils::findInstallDirectories();

    for (const QString &installDirectory : installDirectories) {
        ui->comboBox_install_directory->addItem(installDirectory);
    }
}

void Widget::updateInstallDirectory(const QString &installDirectory)
{
    if (DirUtils::isGameDirectory(installDirectory)) {
        ui->comboBox_install_directory->setCurrentText(installDirectory);
    }
}

void Widget::populateComboboxWithNetworkInterfaces() const
{
    // Loop thru all of the systems network interfaces.
    for (const QNetworkInterface &interface : QNetworkInterface::allInterfaces()) {
        const QNetworkInterface::InterfaceFlags &flags = interface.flags();

        // Only show active network interfaces and not loopback interfaces.
        if (flags.testFlag(QNetworkInterface::IsUp) && !flags.testFlag(QNetworkInterface::IsLoopBack)) {
            QNetworkAddressEntry selectedAddressEntry;

            // Scan thru addresses for this interface.
            for (const QNetworkAddressEntry &addressEntry : interface.addressEntries()) {
                // Only select first IPv4 address found.
                if (addressEntry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
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
    ui->pushButton_patch->setText(!patched ? tr("Install patch") : tr("Uninstall patch"));
}

void Widget::pushButton_install_directory_clicked()
{
    QString installDirectory = QFileDialog::getExistingDirectory(this, tr("Select the %1 installation directory").arg(game_name), ui->comboBox_install_directory->currentText(), QFileDialog::ReadOnly);

    updateInstallDirectory(installDirectory);
}

void Widget::comboBox_network_interface_currentIndexChanged(int index)
{
    QString installDirectory = getInstallDirectory();

    // Only update network configuration if game is patched.
    if (Patcher::isPatched(installDirectory)) {
        // Generate network configuration.
        Patcher::generateNetworkConfigFile(installDirectory, ui->comboBox_network_interface->itemData(index).value<QNetworkAddressEntry>());
    }
}

void Widget::pushButton_patch_clicked()
{
    // Create path to binary folder.
    QDir path = getInstallDirectory();

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
