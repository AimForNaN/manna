#include "library.h"
#include "module.h"
#include "module_bible.h"

#include <QJsonArray>
#include <QJsonObject>

#include <swmgr.h>
#include <swmodule.h>

manna::library::library(const QString &lib, QObject *parent)
	: QObject(parent),
	  lib(new sword::SWMgr(lib.toUtf8().constData()))
{
	loadModules();
}

manna::module manna::library::getModule(const QString &mod) {
	sword::SWModule * m = lib->getModule(mod.toUtf8().constData());
	return module(m);
}

QList<manna::module> manna::library::getModules() {
	QList<module> arr;
	sword::ModMap::iterator it;
	for (it = lib->Modules.begin(); it != lib->Modules.end(); ++it) {
		sword::SWModule * mod = (*it).second;
		module mm = module::fromType(mod);
		arr.append(mm);
	}
	return arr;
}

void manna::library::loadModules() {
    lib->load();
}
