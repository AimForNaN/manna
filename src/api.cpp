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

Manna::Api::Api(QString url, QObject * parent)
	: QObject(parent),
	  lib(new Library(NULL, this)),
	  url(url)
{
	connect(this, SIGNAL(httpGetRequest(mg_connection*,http_message*)), this, SLOT(handleGetRequest(mg_connection*,http_message*)));
}

void Manna::Api::handleGetRequest(mg_connection * conn, http_message * msg) {
	QString uri = QString(msg->uri.p).left(msg->uri.len);
	if (uri.startsWith(this->url)) {
		QString call = uri.mid(this->url.length());
		if (call.startsWith("/modules")) {
			QJsonDocument doc = handleModulesGetApi(conn, msg);
			QString content(doc.toJson());
			mg_send_head(conn, 200, content.toUtf8().size(), "Content-Type: application/json");
			mg_printf(conn, content.toUtf8().constData());
		}
	}
	else {
		if (uri == "/license") {
			QFile file("://LICENSE");
			if (file.open(QFile::ReadOnly | QFile::Text)) {
				QString content(file.readAll());
				file.close();

				mg_send_head(conn, 200, content.length(), "");
				mg_printf(conn, content.toUtf8().constData());
			}
			else mg_http_send_error(conn, 404, "Not Found");
		}
		else {
			mg_http_send_error(conn, 404, "Not Found");
		}
	}
}

void Manna::Api::handlePostRequest(mg_connection *, http_message *) {
}

void Manna::Api::handlePutRequest(mg_connection *, http_message *) {
}

QJsonDocument Manna::Api::handleModulesGetApi(mg_connection * conn, http_message * msg) {
	QJsonDocument doc;

	QString uri      = QString(msg->uri.p).left(msg->uri.len);
	QString queryStr = QString(msg->query_string.p).left(msg->query_string.len);

	QUrl url;
	url.setPath(uri);
	url.setQuery(queryStr);

	QUrlQuery query(url);

	QList<Module> mods = lib->getModules();
	if (url.path() == this->url + "/modules") {
		QJsonArray ret;
		auto prepMod = [query] (Module &mod) -> Module {
			// Getting a module by name? Filters are therefore irrelevant!
			if (query.hasQueryItem("Get")) {
				QStringList get = query.allQueryItemValues("Get", QUrl::FullyDecoded);
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

			return Module(NULL);
		};
		for (int i = 0; i < mods.length(); ++i) {
			Module mod = mods.at(i);
			mod = prepMod(mod);
			if (!mod.isNull()) {
				// Set a key to all modules pulled if there is one
				if (query.hasQueryItem("Key")) {
					QString key = query.queryItemValue("Key", QUrl::FullyDecoded);
					mod.setKey(key);
				}
				ret.append(mod.toJson());
			}
		}
		doc.setArray(ret);
	}

	return doc;
}
