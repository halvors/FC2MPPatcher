
#include <QLineEdit>
#include <QDir>
#include <QMessageBox>
#include <QStringList>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QFileDialog>

#include "widget.h"
#include "ui_widget.h"
#include "installdir.h"
#include "fileutils.h"
#include "netutils.h"
#include "patcher.h"

#include <QSysInfo>

Widget::Widget(const QString &installDir, const QString &interfaceName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QByteArray test = QSysInfo::machineUniqueId();
    qDebug().noquote() << tr("ID: %1").arg(test.constData());

    // Set window title.
    setWindowTitle(QString("%1 %2%3").arg(app_name).arg(APP_VERSION).arg(DEBUG_MODE ? "-dev" : QString()));

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
    loadSettings(installDir, interfaceName);

    // Update patch button according to patch status.
    const QString &path = getInstallDirectory(false);
    Patcher::State patched = Patcher::isPatched(path);
    updatePatchStatus(patched);

    // Register GUI signals to slots.
    connect(ui->comboBox_install_directory,   QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::comboBox_install_directory_currentIndexChanged);
    connect(ui->pushButton_install_directory, &QPushButton::clicked,                               this, &Widget::pushButton_install_directory_clicked);
    connect(ui->comboBox_network_interface,   QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::comboBox_network_interface_currentIndexChanged);
    connect(ui->pushButton_patch,             &QPushButton::clicked,                               this, &Widget::pushButton_patch_clicked);

    // Register signals to saveSettings slot.
    connect(ui->comboBox_install_directory,   QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::saveSettings);
    connect(ui->comboBox_network_interface,   QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::saveSettings);
    connect(ui->pushButton_patch,             &QPushButton::clicked,                               this, &Widget::saveSettings);
}

Widget::~Widget()
{
    delete ui;
    delete settings;
}

void Widget::closeEvent(QCloseEvent *event)
{
    saveSettings();

    QWidget::closeEvent(event);
}

void Widget::loadSettings(const QString &installDir, const QString &interfaceName)
{
    QDir dir = settings->value(settings_install_directory).toString();

    if (!installDir.isEmpty())
        dir = installDir;

    if (InstallDir::isGameDirectory(dir)) {
        const QStringList &installDirectories = InstallDir::findInstallDirectories();

        // If we're in executable directory, cd up to install directory.
        if (dir.dirName() == game_executable_directory)
            dir.cdUp();

        // Avoid duplicate install directories.
        if (!installDirectories.contains(dir.absolutePath())) {
            // Insert item first in list.
            ui->comboBox_install_directory->insertItem(0, dir.absolutePath());

            // Set current index to first element in list.
            ui->comboBox_install_directory->setCurrentIndex(0);
        }
    }

    QNetworkInterface selectedNetworkInterface = settings->value(settings_network_interface).value<QNetworkInterface>();

    if (!interfaceName.isEmpty())
        selectedNetworkInterface = QNetworkInterface::interfaceFromName(interfaceName);

    if (selectedNetworkInterface.isValid())
        for (int i = 0; i < ui->comboBox_network_interface->count(); i++) {
            const QNetworkInterface &networkInterface = ui->comboBox_network_interface->itemData(i).value<QNetworkInterface>();

            if (networkInterface.name() == selectedNetworkInterface.name())
                ui->comboBox_network_interface->setCurrentIndex(i);
        }

    settings->beginGroup(settings_group_window);
        resize(settings->value(settings_group_window_size, size()).toSize());
        move(settings->value(settings_group_window_position, pos()).toPoint());

        if (settings->value(settings_group_window_isMaximized, false).toBool())
            showMaximized();
    settings->endGroup();
}

void Widget::saveSettings() const
{
    const QString &path = ui->comboBox_install_directory->currentText();

    if (InstallDir::isGameDirectory(path)) {
        settings->setValue(settings_install_directory, path);
    }

    settings->setValue(settings_network_interface, ui->comboBox_network_interface->currentData().value<QNetworkInterface>().name());
    settings->beginGroup(settings_group_window);
        settings->setValue(settings_group_window_size, size());
        settings->setValue(settings_group_window_position, pos());
        settings->setValue(settings_group_window_isMaximized, isMaximized());
    settings->endGroup();
}

QString Widget::getInstallDirectory(bool warning)
{
    // Create path to binary folder.
    const QDir &dir = ui->comboBox_install_directory->currentText();

    if (InstallDir::isGameDirectory(dir)) {
        return dir.absolutePath();
    }

    if (warning) {
        QMessageBox::warning(this, "Warning", tr("%1 installation directory not found, please select it manually.").arg(game_name));
    }

    return QString();
}

void Widget::populateComboboxWithInstallDirectories() const
{
    for (const QString &path : InstallDir::findInstallDirectories()) {
        ui->comboBox_install_directory->addItem(path);
    }
}

void Widget::populateComboboxWithNetworkInterfaces() const
{
    const QList<QNetworkInterface> &interfaces = NetUtils::getValidInterfaces();

    // If no network interfaces is found, return early.
    if (interfaces.isEmpty()) {
        ui->comboBox_network_interface->setEnabled(false);
        ui->comboBox_network_interface->addItem(tr("No network interface available."));

        return;
    }

    // Loop thru all of the systems network interfaces.
    for (const QNetworkInterface &interface : interfaces) {
        for (const QNetworkAddressEntry &addressEntry : interface.addressEntries()) {
            const QHostAddress hostAddress = addressEntry.ip();

            // Only select first IPv4 address found.
            if (hostAddress.protocol() == QAbstractSocket::IPv4Protocol) {
                ui->comboBox_network_interface->addItem(interface.humanReadableName() + " (" + hostAddress.toString() + ")", QVariant::fromValue<QNetworkInterface>(interface));
                break;
            }
        }
    }
}

void Widget::updatePatchStatus(const Patcher::State &patched) const
{
    QString text;

    switch (patched) {
    case Patcher::INSTALLED:
        text = tr("Uninstall patch");
        break;

    case Patcher::UPGRADABLE:
        text = tr("Upgrade patch");
        break;

    default:
        text = tr("Install patch");
        break;
    }

    ui->pushButton_patch->setText(text);
}

void Widget::comboBox_install_directory_currentIndexChanged(int index)
{
    Q_UNUSED(index)

    const QString &installDir = getInstallDirectory(false);
    updatePatchStatus(Patcher::isPatched(installDir));
}

void Widget::pushButton_install_directory_clicked()
{
    QString installDir = QFileDialog::getExistingDirectory(this, tr("Please select the %1 installation directory").arg(game_name), ui->comboBox_install_directory->currentText(), QFileDialog::ReadOnly);

    if (!InstallDir::isGameDirectory(installDir)) {
        QMessageBox::information(this, tr("Information"), tr("The selected directory does not contain a %1 installation, please try again!").arg(game_name));

        return;
    }

    ui->comboBox_install_directory->setCurrentText(installDir);
    updatePatchStatus(Patcher::isPatched(installDir));
}

void Widget::comboBox_network_interface_currentIndexChanged(int index)
{
    const QDir &dir = getInstallDirectory();

    // Only update network configuration if game is patched.
    if (Patcher::isPatched(dir.absolutePath()) == Patcher::INSTALLED) {
        // Generate network configuration.
        Patcher::generateConfigurationFile(dir, ui->comboBox_network_interface->itemData(index).value<QNetworkInterface>());
    }
}

void Widget::pushButton_patch_clicked()
{
    // Create path to binary folder.
    const QString &installDir = getInstallDirectory();

    if (installDir.isEmpty()) {
        return;
    }

    QDir dir = installDir;
    dir.cd(game_executable_directory);

    // Only show option to patch if not already patched.
    switch (Patcher::isPatched(dir.absolutePath())) {
    case Patcher::INSTALLED:
        Patcher::undoPatch(dir);
        updatePatchStatus(Patcher::NOT_INSTALLED);
        break;

    case Patcher::UPGRADABLE:
        Patcher::undoPatch(dir);
        [[fallthrough]];

    default:
        // Apply patch to files, if successful continue.
        if (Patcher::patch(dir, this)) {
            // Generate network configuration.
            Patcher::generateConfigurationFile(dir, ui->comboBox_network_interface->currentData().value<QNetworkInterface>());
            updatePatchStatus(Patcher::INSTALLED);
        }
        break;
    }
}
