#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QSettings>
#include <QCloseEvent>

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

private slots:
    void pushButton_install_directory_clicked();
    void pushButton_patch_clicked();

private:
    Ui::Widget* ui;
    QSettings* settings;

    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent* event);

    void populateComboboxWithNetworkInterfaces();
    void populateComboboxWithTargets();
};

#endif // WIDGET_H
