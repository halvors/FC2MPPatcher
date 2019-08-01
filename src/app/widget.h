#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
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

private:
    Ui::Widget* ui;
    QSettings* settings;

    void loadSettings();
    void saveSettings() const;
    void closeEvent(QCloseEvent* event);

    QString findPath();
    QString getPath();
    void populateComboboxWithNetworkInterfaces() const;
    void updatePatchStatus(bool patched) const;

private slots:
    void pushButton_install_directory_clicked();
    void pushButton_patch_clicked();
};

#endif // WIDGET_H
