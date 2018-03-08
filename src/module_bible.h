#ifndef MODULE_BIBLE_H
#define MODULE_BIBLE_H

#include "module.h"

namespace sword {
	class SWKey;
	class SWModule;
}

namespace manna {
	class module;

	class bible : public module
	{
		Q_OBJECT

		public:
			explicit bible(sword::SWModule *, QObject * = 0);
			bible(const module &);

			QJsonObject getStructure();
			QJsonArray renderText();

		protected:
			QJsonObject prepareVerse(const sword::SWKey *);
	};
}

#endif // MODULE_BIBLE_H
