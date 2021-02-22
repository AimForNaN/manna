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

manna::api::api(QObject * parent)
	: QObject(parent)
	, lib(new library("", this))
{
    this->handlers.insert(new handler("GET", "/license", [this] (connection &conn) {
        this->getLicense(conn);
    }));

    this->handlers.insert(new handler("POST", "/v1/library/refresh", [this] (connection &conn) {
        lib->loadModules();
        this->getModules(conn);
    }));

    this->handlers.insert(new handler("PUT", "/v1/modules/unlock", [this] (connection &conn) {
        this->unlockModules(conn);
    }));

    this->handlers.insert(new handler("GET", "/v1/modules", [this] (connection &conn) {
        this->getModules(conn);
    }));
}

manna::handle manna::api::getHandler(std::string method, std::string path) {
	QSetIterator<handler*> i(this->handlers);
	while (i.hasNext()) {
	    handler *h = i.next();
	    if (h->matches(method, path)) {
            return h->operator()(); // Not sure why the shorthand doesn't compile!
	    }
	}
	return nullptr;
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

    QUrl::ComponentFormattingOptions FullyDecoded = QUrl::FullyDecoded;

	QList<module> mods = lib->getModules();
	QJsonArray ret;
    auto prepMod = [query, FullyDecoded] (module &mod) -> module {
		// Getting a module by name? Filters are therefore irrelevant!
		if (query.hasQueryItem("Names")) {
            QStringList get = query.allQueryItemValues("Names", FullyDecoded);
			if (get.contains(mod.getName())) {
				return mod;
			}
		}
		else {
			if (query.hasQueryItem("Type")) {
                QString type = query.queryItemValue("Type", FullyDecoded);
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
                QString key = query.queryItemValue("Key", FullyDecoded);
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

void manna::api::unlockModules(connection &) {
}
