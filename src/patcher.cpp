#include "patcher.h"

Patcher::Patcher(QObject *parent) : QObject(parent), peFile(new PeFile(this))
{

}

Patcher::~Patcher()
{
    delete peFile;
}

void Patcher::applyPatch(const QString &path, const QString &target)
{
    for (QString &functionName : Constants::targets.value(target).keys()) {
        peFile->addFunction(Constants::library_name, functionName);
    }

    peFile->load(path, target);
    peFile->apply();
    peFile->save();
}
