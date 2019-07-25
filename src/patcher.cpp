#include "patcher.h"

Patcher::Patcher(QObject *parent) : QObject(parent), pe(new PeFile(this))
{

}

Patcher::~Patcher()
{
    delete pe;
}

void Patcher::applyPatch(const QString &fileName)
{
    // Load the file into this program.
    pe->addFunction(Constants::library_name, Constants::library_function_getAdaptersInfo);
    pe->addFunction(Constants::library_name, Constants::library_function_getHostbyname);
    pe->apply(fileName);
}
