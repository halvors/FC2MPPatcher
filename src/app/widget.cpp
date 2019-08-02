#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

#include "widget.h"
#include "ui_widget.h"
#include "file.h"
#include "patcher.h"

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

    bool patched = Patcher::isPatched(getPath());
    updatePatchStatus(patched);

    connect(ui->pushButton_install_directory, &QPushButton::clicked, this, &Widget::pushButton_install_directory_clicked);
    connect(ui->lineEdit_install_directory, &QLineEdit::textChanged, this, &Widget::saveSettings);
    connect(ui->comboBox_network_interface, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::comboBox_network_interface_currentIndexChanged);
    connect(ui->pushButton_patch, &QPushButton::clicked, this, &Widget::pushButton_patch_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent* event)
{
    QWidget::closeEvent(event);

    saveSettings();
}

void Widget::loadSettings()
{
    QString install_directory = settings->value(Constants::settings_install_directory, Constants::game_install_directory).toString();
    ui->lineEdit_install_directory->setText(install_directory);

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
    settings->setValue(Constants::settings_install_directory, ui->lineEdit_install_directory->text());
    settings->setValue(Constants::settings_interface_index, ui->comboBox_network_interface->currentIndex());

    settings->beginGroup(Constants::settings_group_window);
        settings->setValue(Constants::settings_group_window_size, size());
        settings->setValue(Constants::settings_group_window_position, pos());
        settings->setValue(Constants::settings_group_window_isMaximized, isMaximized());
    settings->endGroup();
}

QString Widget::findPath()
{
    QString installDirectory = QString();

#ifdef Q_OS_WIN
    // Extract Far Cry 2 registry installdir here.
    QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE", QSettings::NativeFormat);
    registry.beginGroup("7-Zip");

    qDebug() << "Registry:" << registry.value("Path").toString();

    registry.endGroup();
#endif

    return installDirectory;
}


QString Widget::getPath()
{
    // Create path to binary folder.
    QString path = ui->lineEdit_install_directory->text() + "/" + Constants::game_executable_directory + "/";

    // Verify that binary folder actually exists.
    if (!QDir(path).exists()) {
        QMessageBox::warning(this, "Warning", Constants::game_name + " installation directory not found, please select it manually.");
    }

    return path;
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
    QString installDirectory = QFileDialog::getExistingDirectory(this, "Select the " + Constants::game_name + " installation directory", ui->lineEdit_install_directory->text(), QFileDialog::ReadOnly);

    if (QDir(installDirectory).exists()) {
        ui->lineEdit_install_directory->setText(installDirectory);
    }
}

void Widget::comboBox_network_interface_currentIndexChanged(int index)
{
    // Generate network configuration.
    Patcher::generateNetworkConfigFile(getPath(), ui->comboBox_network_interface->itemData(index).value<QNetworkAddressEntry>());
}

void Widget::pushButton_patch_clicked()
{
    // Create path to binary folder.
    QString path = getPath();

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
