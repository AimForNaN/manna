#include "server.h"
#include "api.h"

#include <QDebug>

Manna::Api * Manna::Server::Api  = new Manna::Api("/manna/v1");
const char * Manna::Server::Port = "7777";

mg_connection * Manna::Server::conn = NULL;
mg_mgr Manna::Server::server;
mg_serve_http_opts Manna::Server::serverOpts;

void Manna::Server::httpHandler(mg_connection *nc, int ev, void *p) {
	if (ev == MG_EV_HTTP_REQUEST) {
		http_message * msg = (http_message *) p;

		QString method = QString(msg->method.p).left(msg->method.len);
		if (method == "GET") {
			emit Api->httpGetRequest(nc, msg);
		}
		else if (method == "PUT") {
			emit Api->httpPutRequest(nc, msg);
		}
		else mg_http_send_error(nc, 400, "Unsupported method!");
	}
}

void Manna::Server::poll() {
	for (;;) {
		mg_mgr_poll(&server, 1000);
	}
}

void Manna::Server::start() {
	mg_mgr_init(&server, NULL);
	conn = mg_bind(&server, Port, &httpHandler);
	if (conn) {
		mg_set_protocol_http_websocket(conn);
		serverOpts.document_root = ".";
		serverOpts.enable_directory_listing = "no";
	}
}

void Manna::Server::stop() {
	mg_mgr_free(&server);
}
