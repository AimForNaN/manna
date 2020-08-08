#ifndef MODULE_H
#define MODULE_H

#include <QObject>

struct QDomNode;

namespace sword {
	class SWModule;
}

namespace manna {
	class module : public QObject
	{
		Q_OBJECT

		friend class bible;
        friend class generic_book;

		public:
			explicit module(sword::SWModule *, QObject * = 0);
			module(const module &);
			module& operator =(const module &);

			static module fromType(sword::SWModule *);
			QString getName();
			QJsonObject getStructure();
			QString getType();
			bool isNull();
			bool isRightToLeft();
			void setKey(const QString &);
			QJsonObject toJson();
			QJsonArray renderText();

        protected:
            QJsonObject handleNode(const QDomNode &);

		private:
			QString key;
			sword::SWModule * swmod;
	};
}

#endif // MODULE_H
