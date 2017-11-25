#ifndef MODULE_H
#define MODULE_H

#include <QObject>

namespace sword {
	class SWModule;
}

namespace manna {
	class module : public QObject
	{
		Q_OBJECT

		friend class bible;

		public:
			explicit module(sword::SWModule *, QObject * = 0);
			module(const module &);
			module& operator =(const module &);

			static module fromType(sword::SWModule *);
			QString getName();
			QString getType();
			bool isNull();
			bool isRightToLeft();
			void setKey(const QString &);
			QJsonObject toJson();
			QJsonArray renderText();

		private:
			QString key;
			sword::SWModule * swmod;
	};
}

#endif // MODULE_H
