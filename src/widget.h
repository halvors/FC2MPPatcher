#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QSettings>

#include "constants.h"
#include "fc2mppatcher.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void pushButton_install_dir_clicked();
    void comboBox_network_interface_currentIndexChanged(int index);
    void pushButton_patch_clicked();

private:
    Ui::Widget *ui;
    QSettings *settings;
    FC2MPPatcher *patcher;

    QString install_directory;
    int interfaceIndex;

    void loadSettings();
    void saveSettings();
    void populateComboboxWithNetworkInterfaces();
    bool generateNetworkConfigFile(const QString &address);

};

#endif // WIDGET_H
