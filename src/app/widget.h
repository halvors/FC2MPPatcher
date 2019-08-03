#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>
#include <QCloseEvent>
#include <QDir>

#include "constants.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = nullptr);
    ~Widget();

private:
    Ui::Widget* ui;
    QSettings* settings;

    void closeEvent(QCloseEvent* event);
    void loadSettings();
    void saveSettings() const;

    QString findInstallDir() const;
    QString getInstallDir(bool showWarning = true);
    void populateComboboxWithNetworkInterfaces() const;
    void updatePatchStatus(bool patched) const;

private slots:
    void updateInstallDir(const QString &installDir = QString());

    void pushButton_install_directory_clicked();
    void comboBox_network_interface_currentIndexChanged(int index);
    void pushButton_patch_clicked();
};

#endif // WIDGET_H
