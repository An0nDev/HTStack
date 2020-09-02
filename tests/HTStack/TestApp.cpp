#include "TestApp.hpp"
#include <iostream>
#include <vector>
#include "../../src/Response/Response.hpp"

TestApp::TestApp () {
    std::cout << "[TA] TestApp called" << std::endl;
};

void TestApp::onLoad () {
    std::cout << "[TA] onLoad called" << std::endl;
};

void TestApp::onRequest (HTStack::Request & request) {
    std::cout << "[TA] onRequest called" << std::endl;

    std::cout << "[TA] Client socket: " << request.clientSocket << std::endl;
    std::cout << "[TA] Method number: " << request.method << std::endl;
    std::cout << "[TA] Path: " << request.path << std::endl;

    for (std::pair <std::string, std::string> header : request.headers) {
        std::cout << "[TA] Header " << header.first << " has value " << header.second << std::endl;
    }

    std::string responseString ("Hello from TestApp!");
    std::vector <char> responseData (responseString.begin (), responseString.end ());
    HTStack::Response response (200, responseData);
    response.respondTo (request);

    std::cout << "[TA] onRequest call completed" << std::endl;
};

void TestApp::onUnload () {
    std::cout << "[TA] onUnload called" << std::endl;
};

TestApp::~TestApp () {
    std::cout << "[TA] ~TestApp called" << std::endl;
};

extern "C" HTStack::App* factory () {
    return new TestApp ();
};
