#ifndef MANNA_API_H
#define MANNA_API_H

#include <QObject>
#include <manna-api.h>

namespace manna {
	class connection;
	class library;

	class api : public QObject, public api_interface
	{
		Q_OBJECT

		public:
			api(QString = "/v1", QObject * = 0);

			virtual handler getHandler(std::string,std::string);

		protected:
			void getLicense(connection &);
			void getModules(connection &);

		private:
			library * lib;
			QString url;
	};
}

#endif // MANNA_API_H
