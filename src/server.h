#ifndef MANNA_SERVER_H
#define MANNA_SERVER_H

#include "mongoose.h"

namespace Manna {
	struct Api;

	class Server
	{
		static Manna::Api * Api;
		static const char * Port;

		public:
			static void httpHandler(mg_connection *, int, void *);
			static void poll();
			static void start();
			static void stop();

		private:
			static mg_connection *conn;
			static mg_mgr server;
			static mg_serve_http_opts serverOpts;
	};
}

#endif // MANNA_SERVER_H
