# Manna

Manna is a server application that seeks to serve Biblical scripture and related media via the HTTP/2 protocol.
The goal is to decouple frontend code from backend code,
allowing for more rapid frontend development.

In order for the manna server to communicate with modern browsers,
you must provide the server with a full-chain certificate and a private key.
This will enable the server to be mindful of ALPN negotiations.

Self-signed certificates are perfectly fine for personal use.
You can use the following command to create your own self-signed certificate.

```
openssl req -x509 -newkey rsa:2048 -nodes -sha256 -keyout server.key -out server.crt
```

Place the generated files in the same directory as the server and restart the server.

## Command-line Options

By default, the manna server listens to port 7777 on localhost.
It also looks for `server.key` and `server.crt` in the current working directory.
All of these can be overwritten through the command line.

```
Usage: manna [options] host port

Options:
    -p, --private-key  Set path to private key.
    -c, --certificate  Set path to full-chain certificate.
    -h, --help         Displays help.

Arguments:
    host               Domain to listen to.
    port               Port number to listen to (defaults to 7777).
```

## Licensing

Manna, itself, is licensed under "GNU GPL v2 or later" to meet the requirements of its dependencies that are licensed under "GNU GPL v2 only" and other compatible licenses.

## Third-party dependencies

* Qt5
* manna-server
* libsword
