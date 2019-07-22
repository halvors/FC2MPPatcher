#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>

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
    void pushButton_dir_clicked();
    void pushButton_patch_clicked();

private:
    Ui::Widget *ui;
    FC2MPPatcher *patcher;

    QString install_dir;

    void loadSettings();
    void saveSettings();

};

#endif // WIDGET_H
