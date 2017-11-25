#ifndef MANNA_H
#define MANNA_H

#include <QtCore>

namespace manna {
	class api;
	class server;

	class manna : public QCoreApplication
	{
		Q_OBJECT

		public:
			manna(int &, char **);

		private:
			api * Api = 0;
			server * Server	= 0;
	};
}

#endif // MANNA_H
