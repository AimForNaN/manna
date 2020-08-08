#ifndef MODULE_GENERIC_BOOK_H
#define MODULE_GENERIC_BOOK_H

#include "module.h"

namespace sword {
    class SWKey;
    class SWModule;
}

namespace manna {
    class module;

    class generic_book : public module
    {
        Q_OBJECT

        public:
            explicit generic_book(sword::SWModule *, QObject * = 0);
            generic_book(const module &);

            QJsonObject getStructure();
            QJsonArray renderText();

        protected:
            QJsonObject prepareText(const sword::SWKey *);
    };
}

#endif // MODULE_GENERIC_BOOK_H
