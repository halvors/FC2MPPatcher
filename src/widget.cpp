#include "widget.h"
#include "ui_widget.h"

#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    loadSettings();

    setWindowTitle("FC2MPPatcher");

    ui->lineEdit->setText(install_dir);

    patcher = new FC2MPPatcher(this);

    connect(ui->pushButton_dir, &QPushButton::clicked, this, &Widget::pushButton_dir_clicked);
    connect(ui->pushButton_patch, &QPushButton::clicked, this, &Widget::pushButton_patch_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::loadSettings()
{
    QSettings settings;
    install_dir = settings.value("install_dir").toString(); //"C:/Program Files/Steam/steamapps/common/Far Cry 2"
}

void Widget::saveSettings()
{
    QSettings settings;
    settings.setValue("install_dir", install_dir);
}

void Widget::pushButton_dir_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this);

    install_dir = fileName;
    ui->lineEdit->setText(fileName);

    // TODO: Do this somewhere else.
    saveSettings();
}

void Widget::pushButton_patch_clicked()
{
    ui->pushButton_patch->setEnabled(false);
    ui->pushButton_patch->setText("Patching your broken yet awesome game...");

    // Create path to binary folder.
    QString path = install_dir + "/bin/";

    // Copy original file to other workfile.
    QFile::copy(path + "FarCry2.exe", path + "FarCry2_patched.exe");

    // Load the file into this program.
    patcher->open(path + "FarCry2_patched.exe");

    patcher->addImportFunction("fix.dll", "_Z17GetAdaptersInfoPXP16_IP_ADAPTER_INFOPm@8");
    patcher->addImportFunction("fix.dll", "_Z14getHostbyname2Pc@4");
    patcher->dumpImportDirectory();

    //ui->pushButton_2->setText("Your game is now fixed! Enjoy the nostalgia of playing...");
}
