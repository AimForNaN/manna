#include "manna.h"

#include <iostream>
#include <manna-server.h>

#include "api.h"

manna::manna::manna(int &argc, char **argv)
	: QCoreApplication(argc, argv)
	, Api(new api)
	, Server(new server)
{
    this->setApplicationName("Manna");

    QCommandLineParser parser;
    parser.setApplicationDescription("Manna API HTTP/2 server");

    parser.addPositionalArgument("host", "Domain to listen to.");
    parser.addPositionalArgument("port", "Port number to listen to (defaults to 7777).");
    parser.addOption({{"p", "private-key"}, "Set path to private key."});
    parser.addOption({{"c", "certificate"}, "Set path to full-chain certificate."});

    const QCommandLineOption help = parser.addHelpOption();

    parser.process(*this);

    if (parser.isSet(help)) {
        parser.showHelp();
        this->quit();
        return;
    }

    QStringList args = parser.positionalArguments();
    QString host;
    QString port;
    QString pk = parser.value("private-key");
    QString crt = parser.value("certificate");

    if (!args.isEmpty()) {
        host = args.at(0);
        port = args.at(1);
    }

    if (host.isEmpty()) {
        host = "localhost";
    }
    if (port.isEmpty()) {
        port = "7777";
    }

    std::cout << pk.toStdString() << crt.toStdString();

	Server->Api  = Api;
	Server->Host = host.toStdString();
	Server->Port = port.toInt();

	if (!pk.isEmpty()) {
	    Server->PrivateKey = pk.toStdString();
	}

	if (!crt.isEmpty()) {
	    Server->Certificate = crt.toStdString();
	}

	Server->run();
}
