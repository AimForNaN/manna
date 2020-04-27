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
	QString url = QString::fromStdString(path);
	if (method == "GET") {
		if (url == "/license") {
			return [this] (connection &conn) {
				this->getLicense(conn);
			};
		}
		else if (url.startsWith(this->url)) {
			QString call = url.mid(this->url.length());
			if (call.startsWith("/modules")) {
				return [this] (connection &conn) {
					this->getModules(conn);
				};
			}
		}
	}
	return NULL;
}

void manna::api::getLicense(connection &conn) {
	manna::response &rsp = conn.Response;
	QFile file("://LICENSE");
	if (file.open(QFile::ReadOnly | QFile::Text)) {
		QString content(file.readAll());
		file.close();

		rsp.Status = 200;
		rsp.Body = content.toStdString();
	}
	else {
		rsp.Status = 404;
	}
	rsp.flush();
}

void manna::api::getModules(connection & conn) {
	manna::request  &req = conn.Request;
	manna::response &rsp = conn.Response;

	QJsonDocument doc;

	std::string path  = req.getPath();
	std::string queryStr = req.getQuery();

	QUrl url;
	url.setPath(QString::fromStdString(path).left(path.length()));
	url.setQuery(QString::fromStdString(queryStr).left(queryStr.length()));

	QUrlQuery query(url);

	QList<module> mods = lib->getModules();
	QJsonArray ret;
	auto prepMod = [query] (module &mod) -> module {
		// Getting a module by name? Filters are therefore irrelevant!
		if (query.hasQueryItem("Names")) {
			QStringList get = query.allQueryItemValues("Names", QUrl::FullyDecoded);
			if (get.contains(mod.getName())) {
				return mod;
			}
		}
		else {
			if (query.hasQueryItem("Type")) {
				QString type = query.queryItemValue("Type", QUrl::FullyDecoded);
				if (type == mod.getType()) {
					return mod;
				}
			}
			else return mod;
		}
		return module(NULL);
	};
	for (int i = 0; i < mods.length(); ++i) {
		module mod = mods.at(i);
		mod = prepMod(mod);
		if (!mod.isNull()) {
			// Set a key to all modules pulled if there is one
			if (query.hasQueryItem("Key")) {
				QString key = query.queryItemValue("Key", QUrl::FullyDecoded);
				mod.setKey(key);
                // Must replace "+" with " " from key!
                mod.setKey(key.replace("+", " "));
			}
			ret.append(mod.toJson());
		}
	}
	doc.setArray(ret);

	rsp.Status = 200;
	rsp.Body = doc.toJson().toStdString();
	rsp.flush();
}
