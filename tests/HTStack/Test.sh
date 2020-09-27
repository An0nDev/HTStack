#!/bin/bash
SRC_DIR=../../src
LD_LIBRARY_PATH=/usr/local/

if [ ! -f cert.pem ] && [ ! -f key.pem ]; then
    openssl req -x509 -nodes -days 365 -newkey rsa:4096 -keyout key.pem -out cert.pem
fi

clang++ \
    -std=c++17 \
    -fPIC -shared \
    -lssl \
    $SRC_DIR/App/App.cpp \
    $SRC_DIR/Request/Request.cpp \
    $SRC_DIR/Response/Response.cpp \
    $SRC_DIR/HTTPUtils/MIMEType.cpp \
    $SRC_DIR/CInteropUtils/CInteropUtils.cpp \
    TestApp.cpp -o TestApp.so

clang++ \
    -std=c++17 \
    -ldl \
    -lpthread \
    -lssl -lcrypto \
    $SRC_DIR/ExceptionUtils/ExceptionUtils.cpp \
    $SRC_DIR/SSL/SSLSetupVars.cpp \
    $SRC_DIR/HTTPUtils/MIMEType.cpp \
    $SRC_DIR/CInteropUtils/CInteropUtils.cpp \
    $SRC_DIR/ClientSocket/SSLClientWrapper.cpp \
    $SRC_DIR/ClientSocket/SSLSocket.cpp \
    $SRC_DIR/ClientSocket/ClientSocket.cpp \
    $SRC_DIR/SocketClientManager/ClientThreadTask.cpp \
    $SRC_DIR/SocketClientManager/ClientThread.cpp \
    $SRC_DIR/SocketClientManager/ClientThreadPool.cpp \
    $SRC_DIR/SocketClientManager/SocketClientManager.cpp \
    $SRC_DIR/SocketManager/SocketManager.cpp \
    $SRC_DIR/Server/Server.cpp \
    $SRC_DIR/App/App.cpp \
    $SRC_DIR/AppLoader/AppContainer.cpp \
    $SRC_DIR/AppLoader/AppLoader.cpp \
    $SRC_DIR/RequestReader/InternalReader.cpp \
    $SRC_DIR/RequestReader/RequestReader.cpp \
    $SRC_DIR/Response/Response.cpp \
    $SRC_DIR/Request/Request.cpp \
    $SRC_DIR/ServerConfiguration/ServerConfiguration.cpp \
    TestServer.cpp -o TestServer \
    && LD_LIBRARY_PATH=. ./TestServer
