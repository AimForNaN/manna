#include <QDebug>

#include "src/manna.h"

int main(int argc, char *argv[])
{
	manna::manna app(argc, argv);
	return app.exec();
}
