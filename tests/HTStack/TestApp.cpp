#include "TestApp.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include "../../src/Response/Response.hpp"

TestApp::TestApp (HTStack::Server & server, std::map <std::string, std::string> & settings) : App (server, settings) {
    std::cout << "[TA] TestApp called" << std::endl;
};

void TestApp::handleRequest (HTStack::Request & request) {
    std::cout << "[TA] handleRequest called" << std::endl;

    std::cout << "[TA] Client socket: " << request.clientSocket << std::endl;
    std::cout << "[TA] Method number: " << request.method << std::endl;
    std::cout << "[TA] Path: " << request.path << std::endl;

    for (std::pair <std::string, std::string> header : request.headers) {
        std::cout << "[TA] Header " << header.first << " has value " << header.second << std::endl;
    }
    if (request.path == "/") {
        std::ifstream testFileStream ("Test.txt", std::ifstream::binary);
        HTStack::MIMEType mimeType ("Test.txt", true); // guess
        HTStack::Response response (200, &testFileStream, &mimeType); // 200 = OK
        response.respondTo (request);
    } else if (request.path == "/logo") {
        std::ifstream logoFileStream ("logo.png", std::ifstream::binary);
        HTStack::MIMEType mimeType ("logo.png", true); // guess
        HTStack::Response response (200, &logoFileStream, &mimeType); // 200 = OK
        response.respondTo (request);
    } else {
        HTStack::Response response (404);
        response.respondTo (request);
    }

    std::cout << "[TA] handleRequest call completed" << std::endl;
};

void TestApp::handleSettingsUpdate (std::string const & key) {
    std::cout << "[TA] Setting " << key << " updated to " << settings [key] << "!" << std::endl;
};

void TestApp::handleSettingsRemove (std::string const & key) {
    std::cout << "[TA] Setting " << key << " removed!" << std::endl;
};

TestApp::~TestApp () {
    std::cout << "[TA] ~TestApp called" << std::endl;
};

extern "C" HTStack::App* factory (HTStack::Server & server, std::map <std::string, std::string> & settings) {
    return new TestApp (server, settings);
};
