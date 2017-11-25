#include "manna.h"

#include <manna-server.h>

#include "src/api.h"

manna::manna::manna(int &argc, char **argv)
	: QCoreApplication(argc, argv)
	, Api(new api)
	, Server(new server)
{
	this->setApplicationName("Manna");

	Server->Api  = Api;
	Server->Host = "localhost";
	Server->Port = 777;

	Server->run();
}
