#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QSettings>
#include <QAction>
#include <QCloseEvent>

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
    void pushButton_install_directory_clicked();
    void pushButton_patch_clicked();

private:
    void closeEvent(QCloseEvent *event);

    Ui::Widget *ui;
    QSettings *settings;
    FC2MPPatcher *patcher;

    QString install_directory;

    void loadSettings();
    void saveSettings();
    bool generateNetworkConfigFile(const QString &installDir, const QString &address);

    void populateComboboxWithNetworkInterfaces();

};

#endif // WIDGET_H
