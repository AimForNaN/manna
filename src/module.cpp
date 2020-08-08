#include "module.h"
#include "module_bible.h"
#include "module_generic_book.h"

#include <QDebug>
#include <QDomDocument>
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
    QString type(mod->getType());
    if (type == sword::SWMgr::MODTYPE_BIBLES) {
		return bible(mod);
    } else if (type == sword::SWMgr::MODTYPE_GENBOOKS) {
        return generic_book(mod);
    }
	return module(mod);
}

QString manna::module::getName() {
	return swmod->getName();
}

QJsonObject manna::module::getStructure() {
    QString type(this->getType());
    if (type == sword::SWMgr::MODTYPE_BIBLES) {
		bible mod = *this;
		return mod.getStructure();
    } else if (type == sword::SWMgr::MODTYPE_GENBOOKS) {
        generic_book mod = *this;
        return mod.getStructure();
    }
	return QJsonObject();
}

QString manna::module::getType() {
	return swmod->getType();
}

QJsonObject manna::module::handleNode(const QDomNode &el) {
    QJsonObject obj;
    if (el.isText()) {
        QString value = el.nodeValue();
        if (!value.isEmpty()) {
            obj["Type"] = "text";
            obj["Text"] = value;
        }
    } else if (el.isElement()) {
        obj["Tag"] = el.nodeName();
        obj["Type"] = "node";

        if (el.hasAttributes()) {
            QJsonObject attrs;
            QDomNamedNodeMap attributes = el.attributes();
            for (int i = 0; i < attributes.length(); ++i) {
                QDomNode a = attributes.item(i);
                attrs[a.nodeName()] = a.nodeValue();
            }
            obj["Attributes"] = attrs;
        }

        if (el.hasChildNodes()) {
            QJsonArray children;
            QDomNodeList childNodes = el.childNodes();
            for (int i = 0; i < childNodes.length(); ++i) {
                children.append(handleNode(childNodes.at(i)));
            }
            obj["Children"] = children;
        }
    }
    return obj;
}

bool manna::module::isNull() {
	return !swmod;
}

bool manna::module::isRightToLeft() {
	return swmod->getConfig().has("Direction", "RtoL");
}

QJsonArray manna::module::renderText() {
    QString type(this->getType());
    if (type == sword::SWMgr::MODTYPE_BIBLES) {
		bible mod = *this;
		return mod.renderText();
    } else if (type == sword::SWMgr::MODTYPE_GENBOOKS) {
        generic_book mod = *this;
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
