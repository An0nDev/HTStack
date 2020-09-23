set -a

if [ ! -f cert.pem ] && [ ! -f key.pem ]; then
    openssl req -x509 -nodes -days 365 -newkey rsa:4096 -keyout key.pem -out cert.pem
fi

clang++ $(pkg-config --libs openssl) \
TLSTest.cpp -o TLSTest
./TLSTest
