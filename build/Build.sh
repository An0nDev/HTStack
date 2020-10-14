#!/bin/bash

cd $(dirname "$0")

SRC_DIR=../src
LD_LIBRARY_PATH=/usr/local/
CPP10_BASE_DIR=~/Downloads/cpp10/out
if [ -z "$COMPILER" ]; then COMPILER=clang++-11; fi
export CFLAGS="-I$CPP10_BASE_DIR/usr/include/ -I$CPP10_BASE_DIR/usr/include/c++/10/"
export LDFLAGS="-L$CPP10_BASE_DIR/usr/lib/gcc/ -L$CPP10_BASE_DIR/usr/lib/gcc/x86_64-linux-gnu/10/"

$COMPILER \
    -std=c++2a \
    -ldl \
    -lpthread \
    -lssl -lcrypto \
    -fPIC -shared \
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
    $SRC_DIR/ServerTerminalWrapper/ServerTerminalWrapper.cpp \
    $SRC_DIR/Server/Server.cpp \
    $SRC_DIR/App/App.cpp \
    $SRC_DIR/AppConfigLoader/StaticAppConfig.cpp \
    $SRC_DIR/AppConfigLoader/AppConfigLoader.cpp \
    $SRC_DIR/AppLoader/AppContainer.cpp \
    $SRC_DIR/AppLoader/AppLoader.cpp \
    $SRC_DIR/RequestReader/InternalReader.cpp \
    $SRC_DIR/RequestReader/RequestReader.cpp \
    $SRC_DIR/Response/Response.cpp \
    $SRC_DIR/Request/Request.cpp \
    $SRC_DIR/ServerConfiguration/ServerConfigLoader.cpp \
    $SRC_DIR/ServerConfiguration/ServerConfiguration.cpp \
    -o ../dist/libHTStack.so

sudo cp ../dist/libHTStack.so /usr/local/lib/

sudo ldconfig

$COMPILER \
    -std=c++2a \
    HTStackServer.cpp \
    -o ../dist/HTStackServer \
    -lHTStack

sudo cp ../dist/HTStackServer /usr/local/bin/

sudo rm -rdf /usr/local/include/HTStack/
sudo mkdir -p /usr/local/include/HTStack/
sudo cp -r ../src/* /usr/local/include/HTStack/

cd $OLDPWD
