#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>
#include <QCloseEvent>
#include <QString>

#include "defs.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(const QString &installDir = QString(), const QString &interfaceName = QString(), QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QSettings *settings;

    void closeEvent(QCloseEvent *event);
    void loadSettings(const QString &installDir, const QString &interfaceName);

    QString getInstallDirectory(bool warning = true);
    void populateComboboxWithInstallDirectories() const;
    void populateComboboxWithNetworkInterfaces() const;
    void updatePatchStatus(bool patched) const;

private slots:
    void saveSettings() const;

    void comboBox_install_directory_currentIndexChanged(int index);
    void pushButton_install_directory_clicked();
    void comboBox_network_interface_currentIndexChanged(int index);
    void pushButton_patch_clicked();
};

#endif // WIDGET_H
