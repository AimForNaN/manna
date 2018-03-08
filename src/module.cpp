#include "module.h"
#include "module_bible.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

#include <swmgr.h>
#include <swmodule.h>

manna::module::module(sword::SWModule *mod, QObject *parent)
	: QObject(parent)
	, swmod(mod)
{
}

manna::module::module(const module &mod) : QObject() {
	this->setParent(mod.parent());
	swmod = mod.swmod;
	key = mod.key;
}

manna::module& manna::module::operator =(const module &mod) {
	swmod = mod.swmod;
	key = mod.key;
	return *this;
}

manna::module manna::module::fromType(sword::SWModule *mod) {
	if (mod->getType() == sword::SWMgr::MODTYPE_BIBLES) {
		return bible(mod);
	}
	return module(mod);
}

QString manna::module::getName() {
	return swmod->getName();
}

QJsonObject manna::module::getStructure() {
	if (!strcmp(swmod->getType(), sword::SWMgr::MODTYPE_BIBLES)) {
		bible mod = *this;
		return mod.getStructure();
	}
	return QJsonObject();
}

QString manna::module::getType() {
	return swmod->getType();
}

bool manna::module::isNull() {
	return !swmod;
}

bool manna::module::isRightToLeft() {
	return swmod->getConfig().has("Direction", "RtoL");
}

QJsonArray manna::module::renderText() {
	if (!strcmp(swmod->getType(), sword::SWMgr::MODTYPE_BIBLES)) {
		bible mod = *this;
		return mod.renderText();
	}
	return QJsonArray();
}

void manna::module::setKey(const QString &key) {
	this->key = key;
}

QJsonObject manna::module::toJson() {
	QJsonObject mod;
	mod["class"]       = "module";
	mod["About"]       = swmod->getConfigEntry("About");
	mod["Description"] = swmod->getConfigEntry("Description");
	mod["Encoding"]    = swmod->getConfigEntry("Encoding");
	mod["Feature"]     = swmod->getConfigEntry("Feature");
	mod["Key"]         = key;
	mod["Language"]    = swmod->getConfigEntry("Lang");
	mod["Name"]        = swmod->getName();
	mod["R2L"]         = isRightToLeft();
	mod["Structure"]   = getStructure();
	mod["Text"]        = renderText();
	mod["Version"]     = swmod->getConfigEntry("Version");
	mod["Type"]        = swmod->getType();
	return mod;
}
