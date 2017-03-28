#include "api.h"

#include <QDebug>
#include <QFile>

Manna::Api::Api(QString url, QObject * parent) : QObject(parent) {
	this->url = url;

	connect(this, SIGNAL(httpGetRequest(mg_connection*,http_message*)), this, SLOT(handleGetRequest(mg_connection*,http_message*)));
}

void Manna::Api::handleGetRequest(mg_connection * conn, http_message * msg) {
	QString uri = QString(msg->uri.p).left(msg->uri.len);
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
