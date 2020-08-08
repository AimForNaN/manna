#ifndef MANNA_API_H
#define MANNA_API_H

#include <QObject>
#include <QSet>
#include <manna-api.h>

namespace manna {
	class connection;
	class library;

	class api : public QObject, public api_interface
	{
		Q_OBJECT

		public:
			api(QObject * = 0);

			virtual handle getHandler(std::string,std::string);

		protected:
			void getLicense(connection &);
			void getModules(connection &);
            void unlockModules(connection &);

		private:
            QSet<handler *> handlers;
			library * lib;
	};
}

#endif // MANNA_API_H
