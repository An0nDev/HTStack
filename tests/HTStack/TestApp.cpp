#include "TestApp.hpp"
#include <iostream>
#include <vector>
#include "../../src/Response/Response.hpp"

TestApp::TestApp () {
    std::cout << "--> TestApp::TestApp () called" << std::endl;
};

void TestApp::onLoad () {
    std::cout << "--> TestApp::onLoad () called" << std::endl;
};

void TestApp::onRequest (HTStack::Request & request) {
    std::cout << "--> TestApp::onRequest (HTStack::Request & request) called" << std::endl;

    std::cout << "--> Client socket: " << request.clientSocket << std::endl;
    std::cout << "--> Method number: " << request.method << std::endl;
    std::cout << "--> Path: " << request.path << std::endl;

    for (std::pair <std::string, std::string> header : request.headers) {
        std::cout << "--> Header " << header.first << " has value " << header.second << std::endl;
    }

    std::string responseString ("Success!");
    std::vector <char> responseData (responseString.begin (), responseString.end ());
    HTStack::Response response (200, responseData);
    response.respondTo (request);
};

void TestApp::onUnload () {
    std::cout << "--> TestApp::onUnload () called" << std::endl;
};

TestApp::~TestApp () {
    std::cout << "--> TestApp::~TestApp () called" << std::endl;
};

extern "C" HTStack::App* factory () {
    return new TestApp ();
};
