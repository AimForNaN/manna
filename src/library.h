#ifndef MANNA_LIBRARY_H
#define MANNA_LIBRARY_H

#include <QObject>

namespace sword {
	class SWMgr;
	class SWModule;
}

namespace Manna {
	class Module;

	class Library : public QObject
	{
		Q_OBJECT

		public:
			Library(const QString &lib = "", QObject * = 0);

			Module getModule(const QString &);
			QList<Module> getModules();

		public slots:
			void loadModules();

		private:
			sword::SWMgr *lib;
	};
}

#endif // MANNA_LIBRARY_H
