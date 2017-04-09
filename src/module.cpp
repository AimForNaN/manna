#include "module.h"
#include "module_bible.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

#include <swmgr.h>
#include <swmodule.h>

Manna::Module::Module(sword::SWModule *mod, QObject *parent)
	: QObject(parent),
	  module(mod)
{
}

Manna::Module::Module(const Module &mod) : QObject() {
	this->setParent(mod.parent());
	module = mod.module;
	key = mod.key;
}

Manna::Module& Manna::Module::operator =(const Module &mod) {
	module = mod.module;
	key = mod.key;
	return *this;
}

Manna::Module Manna::Module::fromType(sword::SWModule *mod) {
	if (mod->getType() == sword::SWMgr::MODTYPE_BIBLES) {
		return ModuleBible(mod);
	}
	return Module(mod);
}

QString Manna::Module::getName() {
	return module->getName();
}

QString Manna::Module::getType() {
	return module->getType();
}

bool Manna::Module::isNull() {
	return !module;
}

bool Manna::Module::isRightToLeft() {
	return module->getConfig().has("Direction", "RtoL");
}

QJsonArray Manna::Module::renderText() {
	if (!strcmp(module->getType(), sword::SWMgr::MODTYPE_BIBLES)) {
		ModuleBible mod = *this;
		return mod.renderText();
	}
	return QJsonArray();
}

void Manna::Module::setKey(const QString &key) {
	this->key = key;
}

QJsonObject Manna::Module::toJson() {
	QJsonObject mod;
	mod["class"]       = "module";
	mod["About"]       = module->getConfigEntry("About");
	mod["Description"] = module->getConfigEntry("Description");
	mod["Encoding"]    = module->getConfigEntry("Encoding");
	mod["Feature"]     = module->getConfigEntry("Feature");
	mod["Key"]         = key;
	mod["Language"]    = module->getConfigEntry("Lang");
	mod["Name"]        = module->getName();
	mod["R2L"]         = isRightToLeft();
	mod["Text"]        = renderText();
	mod["Version"]     = module->getConfigEntry("Version");
	mod["Type"]        = module->getType();
	return mod;
}
