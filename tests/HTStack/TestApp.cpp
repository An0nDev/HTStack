#include "TestApp.hpp"
#include <iostream>

TestApp::TestApp () {
    std::cout << "--> TestApp::TestApp () called" << std::endl;
};

void TestApp::onLoad () {
    std::cout << "--> TestApp::onLoad () called" << std::endl;
};

void TestApp::onRequest (HTStack::Request & request) {
    std::cout << "--> TestApp::onRequest (HTStack::Request & request) called" << std::endl;
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
