#include "module_bible.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

#include <swmodule.h>
#include <versekey.h>

manna::bible::bible(sword::SWModule *mod, QObject *parent)
	: module(mod, parent)
{
}

manna::bible::bible(const module &mod) : module(mod) {
}

QJsonObject manna::bible::getStructure() {
	QJsonObject st;
	sword::VerseKey vk;
	for (int b = 0; b < 2; b++) {
		vk.setTestament(b+1);
		for (int i = 0; i < vk.BMAX[b]; i++) {
			vk.setBook(i+1);
			QJsonObject info;
			info["Chapters"] = vk.getChapterMax();
			info["Index"] = vk.getBook();
			info["Testament"] = vk.getTestament();
			st[vk.getBookName()] = info;
		}
	}
	return st;
}

QJsonObject manna::bible::prepareVerse(const sword::SWKey *sk) {
	sword::VerseKey vk(sk);
	swmod->setKey(sk);
	swmod->renderText(); // Prepare text for rendering!
	QJsonObject vinfo;
	vinfo["class"]   = "verse";
	vinfo["Book"]    = vk.getBookName();
	vinfo["Chapter"] = vk.getChapter();
	vinfo["Text"]    = QString::fromUtf8(swmod->renderText());
	vinfo["Verse"]   = vk.getVerse();
	return vinfo;
}

QJsonArray manna::bible::renderText() {
	QJsonArray arr;
	sword::VerseKey vk;
	sword::ListKey lk = vk.parseVerseList(key.toUtf8().constData(), 0, true);

	QJsonObject verse;
	for (int i = 0; i < lk.Count(); ++i) {
		const sword::SWKey *k = lk.getElement(i);
		sword::VerseKey vky(k);
//		sword::VerseKey lower = vky.getLowerBound();
		sword::VerseKey upper = vky.getUpperBound();
		while (!upper.equals(vky)) {
//			qDebug() << k->getRangeText() << k->isBoundSet();
			verse = prepareVerse((sword::SWKey*)&vky);
			arr.append(verse);
			vky.increment();
		}

		// Get last one!
		verse = prepareVerse((sword::SWKey*)&vky);
		arr.append(verse);
	}
	return arr;
}
