#include <QFile>
#include <QDebug>

#include "patcher.h"

Patcher::Patcher(QObject *parent) : QObject(parent), pe(new Pe(this))
{

}

Patcher::~Patcher()
{
    delete pe;
}

void Patcher::applyPatch(const QString &installDirectory)
{
    // Create path to binary file that is to be patched.
    QString fileName = installDirectory + "/" + Constants::executable_directory + "/FarCry2_patched.exe";

    // Load the file into this program.
    pe->addLibraryFunction(Constants::library_name, Constants::library_function_getAdaptersInfo);
    pe->addLibraryFunction(Constants::library_name, Constants::library_function_getHostbyname);
    pe->apply(fileName);
}
