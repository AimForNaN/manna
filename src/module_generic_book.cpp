#include "module_generic_book.h"

#include <QDebug>
#include <QDomDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <swmodule.h>
#include <treekey.h>

manna::generic_book::generic_book(sword::SWModule *mod, QObject *parent)
    : module(mod, parent)
{
    auto * tk = dynamic_cast<sword::TreeKey *>(swmod->getKey()->clone());
    tk->firstChild();
    setKey(tk->getText());
    delete tk;
}

manna::generic_book::generic_book(const module &mod) : module(mod) {
}

QJsonObject manna::generic_book::getStructure() {
    QJsonObject st;

    std::function<void(sword::TreeKey*)> getGenBookTOC = [&getGenBookTOC, &st] (sword::TreeKey * tk) {
        if (tk->firstChild()) {
            do {
                QString text = tk->getText();
                st[text] = text;

                if (tk->hasChildren()) {
                    getGenBookTOC(tk);
                }
            }
            while (tk->nextSibling());

            tk->parent();
        }
    };

    auto * tk = dynamic_cast<sword::TreeKey *>(swmod->createKey());
    getGenBookTOC(tk);

    delete tk;
    return st;
}

QJsonObject manna::generic_book::prepareText(const sword::SWKey *sk) {
    swmod->setKey(sk);
    swmod->renderText(); // Prepare text for rendering!
    QJsonObject tinfo;
    tinfo["class"] = "text";

    QJsonArray arr;
    QString srcType = swmod->getConfigEntry("SourceType");
    if (srcType.toLower() == "osis" || srcType.toLower() == "thml") {
        QString text = QString::fromUtf8(swmod->getRawEntry());
        QDomDocument doc;
        doc.setContent("<manna>" + text + "</manna>");
        QDomElement dom = doc.documentElement();
        if (dom.hasChildNodes()) {
            QDomNodeList children = dom.childNodes();
            for (int i = 0; i < children.length(); ++i) {
                QDomNode child = children.at(i);
                arr.append(handleNode(child));
            }
        }
    } else {
        QJsonObject obj;
        obj["Text"] = QString::fromUtf8(swmod->renderText());
        obj["Type"] = "text";
        arr.append(obj);
    }
    tinfo["Text"] = arr;

    return tinfo;
}

QJsonArray manna::generic_book::renderText() {
    QJsonArray arr;
    auto * tk = dynamic_cast<sword::TreeKey *>(swmod->createKey());

    tk->setText(key.toUtf8().constData());
    QJsonObject text = prepareText((sword::SWKey*)tk);
    arr.append(text);

    delete tk;
    return arr;
}
