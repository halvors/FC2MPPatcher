#ifndef FC2MPPATCHER_H
#define FC2MPPATCHER_H

#include <QObject>

class FC2MPPatcher : public QObject
{
    Q_OBJECT

public:
    explicit FC2MPPatcher(QObject *parent = nullptr);

signals:

public slots:

private:
    const QString fc2_install_dir = "C:/Program Files/Steam/steamapps/common/Far Cry 2";

};

#endif // FC2MPPATCHER_H
