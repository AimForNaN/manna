#ifndef MANNA_LIBRARY_H
#define MANNA_LIBRARY_H

#include <QObject>

namespace sword {
	class SWMgr;
	class SWModule;
}

namespace manna {
	class module;

	class library : public QObject
	{
		Q_OBJECT

		public:
			library(const QString &lib = "", QObject * = 0);

			module getModule(const QString &);
			QList<module> getModules();

		public slots:
			void loadModules();

		private:
			sword::SWMgr * lib;
	};
}

#endif // MANNA_LIBRARY_H
