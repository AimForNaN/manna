#include "api.h"
#include "library.h"
#include "module.h"

#include <QDebug>
#include <QFile>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrl>
#include <QUrlQuery>

manna::api::api(QString url, QObject * parent)
	: QObject(parent)
	, lib(new library("", this))
	, url(url)
{
}

manna::handler manna::api::getHandler(std::string method, std::string path) {
	return NULL;
}









