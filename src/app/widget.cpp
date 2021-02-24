#include <QVersionNumber>
#include <QLineEdit>
#include <QDir>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QFileDialog>

#include "widget.h"
#include "ui_widget.h"
#include "dirutils.h"
#include "fileutils.h"
#include "patcher.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // Set window title.
    setWindowTitle(QString("%1 %2").arg(app_name).arg(APP_VERSION));

    // Set label text.
    ui->label_installation_directory->setText(tr("Select the %1 installation directory:").arg(game_name));

    // Add placeholder text to lineEdit.
    QLineEdit *lineEdit_install_directory = ui->comboBox_install_directory->lineEdit();
    lineEdit_install_directory->setPlaceholderText(tr("Enter path to install directory..."));

    // Populate comboBox with found install directories.
    populateComboboxWithInstallDirectories();

    // Populate comboBox with detected network interfaces.
    populateComboboxWithNetworkInterfaces();

    // Load settings from configuration file.
    settings = new QSettings(app_configuration_file, QSettings::IniFormat, this);
    loadSettings();

    // Update patch button according to patch status.
    QString path = getInstallDirectory(false);
    bool patched = Patcher::isPatched(path);
    updatePatchStatus(patched);

    // Register GUI signals to slots.
    connect(ui->comboBox_install_directory,     QOverload<int>::of(&QComboBox::currentIndexChanged),    this, &Widget::comboBox_install_directory_currentIndexChanged);
    connect(ui->pushButton_install_directory,   &QPushButton::clicked,                                  this, &Widget::pushButton_install_directory_clicked);
    connect(ui->comboBox_network_interface,     QOverload<int>::of(&QComboBox::currentIndexChanged),    this, &Widget::comboBox_network_interface_currentIndexChanged);
    connect(ui->pushButton_patch,               &QPushButton::clicked,                                  this, &Widget::pushButton_patch_clicked);

    // Register signals to saveSettings slot.
    connect(ui->comboBox_install_directory,     QOverload<int>::of(&QComboBox::currentIndexChanged),    this, &Widget::saveSettings);
    connect(ui->comboBox_network_interface,     QOverload<int>::of(&QComboBox::currentIndexChanged),    this, &Widget::saveSettings);
    connect(ui->pushButton_patch,               &QPushButton::clicked,                                  this, &Widget::saveSettings);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event)
{
    saveSettings();

    QWidget::closeEvent(event);
}

void Widget::loadSettings()
{
    QDir dir = settings->value(settings_install_directory).toString();

    if (DirUtils::isGameDirectory(dir)) {
        const QStringList &installDirectories = DirUtils::findInstallDirectories();

        // If we're in executable directory, cd up to install directory.
        if (dir.dirName() == game_executable_directory) {
            dir.cdUp();
        }

        // Avoid duplicate install directories.
        if (!installDirectories.contains(dir.absolutePath())) {
            // Insert item first in list.
            ui->comboBox_install_directory->insertItem(0, dir.absolutePath());

            // Set current index to first element in list.
            ui->comboBox_install_directory->setCurrentIndex(0);
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
    QString path = ui->comboBox_install_directory->currentText();

    if (DirUtils::isGameDirectory(path)) {
        settings->setValue(settings_install_directory, path);
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
    for (const QString &path : DirUtils::findInstallDirectories()) {
        ui->comboBox_install_directory->addItem(path);
    }
}

void Widget::populateComboboxWithNetworkInterfaces() const
{
    const QList<QNetworkInterface> &interfaces = QNetworkInterface::allInterfaces();

    // If no network interfaces is found, return early.
    if (interfaces.isEmpty()) {
        ui->comboBox_network_interface->setEnabled(false);
        ui->comboBox_network_interface->addItem(tr("No network interface found."));

        return;
    }

    // Loop thru all of the systems network interfaces.
    for (const QNetworkInterface &interface : interfaces) {
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

            ui->comboBox_network_interface->addItem(interface.humanReadableName() + " (" + selectedAddressEntry.ip().toString() + ")", QVariant::fromValue<QNetworkInterface>(interface));
        }
    }
}

void Widget::updatePatchStatus(bool patched) const
{
    ui->pushButton_patch->setText(!patched ? tr("Install patch") : tr("Uninstall patch"));
}

void Widget::comboBox_install_directory_currentIndexChanged(int index)
{
    Q_UNUSED(index)

    QString path = getInstallDirectory(false);
    updatePatchStatus(Patcher::isPatched(path));
}

void Widget::pushButton_install_directory_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Select the %1 installation directory").arg(game_name), ui->comboBox_install_directory->currentText(), QFileDialog::ReadOnly);

    if (!DirUtils::isGameDirectory(path)) {
        QMessageBox::information(this, tr("Information"), tr("Selected directory is not an %1 installation directory, please try again.").arg(game_name));

        return;
    }

    ui->comboBox_install_directory->setCurrentText(path);
    updatePatchStatus(Patcher::isPatched(path));
}

void Widget::comboBox_network_interface_currentIndexChanged(int index)
{
    QDir dir = getInstallDirectory();

    // Only update network configuration if game is patched.
    if (Patcher::isPatched(dir.absolutePath())) {
        // Generate network configuration.
        Patcher::generateConfigurationFile(dir, ui->comboBox_network_interface->itemData(index).value<QNetworkInterface>());
    }
}

void Widget::pushButton_patch_clicked()
{
    // Create path to binary folder.
    QString path = getInstallDirectory();

    if (path.isEmpty()) {
        return;
    }

    QDir dir = path;
    dir.cd(game_executable_directory);

    // Only show option to patch if not already patched.
    if (Patcher::isPatched(dir.absolutePath())) {
        Patcher::undoPatch(dir);

        updatePatchStatus(false);
    } else {
        // Apply patch to files, if successful continue.
        if (Patcher::patch(dir, this)) {
            // Generate network configuration.
            Patcher::generateConfigurationFile(dir, ui->comboBox_network_interface->currentData().value<QNetworkInterface>());

            updatePatchStatus(true);
        }
    }
}
