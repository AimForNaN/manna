#include <QDebug>
#include <QtCore>

#include "src/server.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	Manna::Server::start();
	Manna::Server::poll();
	Manna::Server::stop();

	return app.exec();
}
