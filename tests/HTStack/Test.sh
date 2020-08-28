SRC_DIR=../../src

clang++ \
    -fPIC -shared \
    $SRC_DIR/App/App.cpp \
    TestApp.cpp -o TestApp.so \

clang++ \
    -ldl \
    -lpthread \
    $SRC_DIR/SocketClientManager/SocketClientManager.cpp \
    $SRC_DIR/SocketManager/SocketManager.cpp \
    $SRC_DIR/Server/Server.cpp \
    $SRC_DIR/App/App.cpp \
    $SRC_DIR/AppLoader/AppContainer.cpp \
    $SRC_DIR/AppLoader/AppLoader.cpp \
    $SRC_DIR/RequestReader/RequestReader.cpp \
    $SRC_DIR/Request/Request.cpp \
    $SRC_DIR/ServerConfiguration/ServerConfiguration.cpp \
    TestServer.cpp -o TestServer \
    && ./TestServer
