#ifndef MANNA_API_H
#define MANNA_API_H

#include <QObject>

#include "mongoose.h"

namespace Manna {
	class Api : public QObject
	{
		Q_OBJECT

		public:
			Api(QString, QObject * = 0);

		public slots:
			void handleGetRequest(mg_connection *, http_message *);

		private:
			QString url;

		signals:
			void httpGetRequest(mg_connection *, http_message *);
			void httpPutRequest(mg_connection *, http_message *);
	};
}

#endif // MANNA_API_H
