#ifndef MODULE_H
#define MODULE_H

#include <QObject>

namespace sword {
	class SWModule;
}

namespace Manna {
	class Module : public QObject
	{
		Q_OBJECT

		friend class ModuleBible;

		public:
			explicit Module(sword::SWModule *, QObject * = 0);
			Module(const Module &);
			Module& operator =(const Module &);

			static Module fromType(sword::SWModule *);
			QString getName();
			QString getType();
			bool isNull();
			bool isRightToLeft();
			void setKey(const QString &);
			QJsonObject toJson();
			QJsonArray renderText();

		private:
			QString key;
			sword::SWModule * module;
	};
}

#endif // MODULE_H
