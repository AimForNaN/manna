#ifndef MODULE_BIBLE_H
#define MODULE_BIBLE_H

#include "module.h"

namespace sword {
	class SWKey;
	class SWModule;
}

namespace Manna {
	class Module;

	class ModuleBible : public Module
	{
		Q_OBJECT

		public:
			explicit ModuleBible(sword::SWModule *, QObject * = 0);
			ModuleBible(const Module &);

			QJsonArray renderText();

		protected:
			QJsonObject prepareVerse(const sword::SWKey *);
	};
}

#endif // MODULE_BIBLE_H
