#include "library.h"
#include "module.h"
#include "module_bible.h"

#include <QJsonArray>
#include <QJsonObject>

#include <swmgr.h>
#include <swmodule.h>

Manna::Library::Library(const QString &lib, QObject *parent)
	: QObject(parent),
	  lib(new sword::SWMgr(lib.toUtf8().constData()))
{
	loadModules();
}

Manna::Module Manna::Library::getModule(const QString &mod) {
	sword::SWModule * m = lib->getModule(mod.toUtf8().constData());
	return Module(m);
}

QList<Manna::Module> Manna::Library::getModules() {
	QList<Module> arr;
	sword::ModMap::iterator it;
	for (it = lib->Modules.begin(); it != lib->Modules.end(); ++it) {
		sword::SWModule * mod = (*it).second;
		Module mm = Module::fromType(mod);
		arr.append(mm);
	}
	return arr;
}

void Manna::Library::loadModules() {
	lib->Load();
}
