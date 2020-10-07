#include "ServerTerminalWrapper.hpp"

#include "../Server/Server.hpp"

#include <iostream>

namespace HTStack {
    const std::string ServerTerminalWrapper::tokenDelimiter = " ";
    void ServerTerminalWrapper::handleCommand (
        std::string const & command,
        std::vector <std::string> const & args
    ) {
        if (command == "help") {
            std::cout << "Note: Commands marked with (!) can only be run with a config file." << std::endl;
            std::cout << "help: Display this message." << std::endl;
            std::cout << "add [name] [location]: Add app with name name and shared object at location. (!)" << std::endl;
            std::cout << "load [name]: Load app with name name. (!)" << std::endl;
            std::cout << "show [name]: Show information about app with name name." << std::endl;
            std::cout << "configure [name] [key] [value]: Set configuration value of key to value in app with name name. (!)" << std::endl;
            std::cout << "unconfigure [name] [key]: Remove key from configuration of app with name name." << std::endl;
            std::cout << "unload [name]: Unload app with name name. (!)" << std::endl;
            std::cout << "remove [name]: Remove app with name name. (!)" << std::endl;
            std::cout << "exit: Shut down the server." << std::endl;
            return;
        } else if (command == "add") {
            if (args.size () == 2) {
                server.appLoader.add (args [0], args [1]);
                return;
            }
        } else if (command == "load") {
            if (args.size () == 1) {
                server.appLoader.load (args [0]);
                return;
            }
        } else if (command == "show") {
            if (args.size () == 1) {
                server.appLoader.load (args [0]);
                return;
            }
        } else if (command == "configure") {
            if (args.size () == 3) {
                server.appLoader.configure (args [0], args [1], args [2]);
                return;
            }
        } else if (command == "unconfigure") {
            if (args.size () == 2) {
                server.appLoader.unconfigure (args [0], args [1]);
                return;
            }
        } else if (command == "unload") {
            if (args.size () == 1) {
                server.appLoader.unload (args [0]);
                return;
            }
        } else if (command == "remove") {
            if (args.size () == 1) {
                server.appLoader.remove (args [0]);
                return;
            }
        } else {
            std::cout << "Invalid command! Type \"help\" for help." << std::endl;
            return;
        }
        std::cout << "Invalid command syntax! Type \"help\" for help." << std::endl;
    };
    ServerTerminalWrapper::ServerTerminalWrapper (Server & server_)
    : server (server_) {};
    void ServerTerminalWrapper::prompt () {
        while (true) { // Repeatedly prompt the user for a command
            std::cout << "HTStack> ";

            std::string line;
            std::getline (std::cin, line);

            bool hasCommand = false;
            std::string command;
            std::vector <std::string> args;

            std::string::size_type tokenLocation;
            while (true) {
                tokenLocation = line.find (tokenDelimiter);
                if (tokenLocation == std::string::npos) {
                    if (!line.empty ()) {
                        if (!hasCommand) {
                            hasCommand = true;
                            command = line;
                        } else {
                            args.push_back (line);
                        }
                    }
                    break;
                }
                if (!hasCommand) {
                    hasCommand = true;
                    command = line.substr (0, tokenLocation);
                    line = line.substr (tokenLocation + tokenDelimiter.size ());
                } else {
                    std::string arg = line.substr (0, tokenLocation + tokenDelimiter.size ());
                    args.push_back (arg);
                    line = line.substr (tokenLocation + tokenDelimiter.size ());
                }
            }
            if (!hasCommand) {
                continue;
            }
            std::transform (command.begin (), command.end (), command.begin (), ::tolower);
            if (command == "exit") {
                break;
            }
            try {
                handleCommand (command, args);
            } catch (std::exception const & exception) {
                std::cerr << "An error occured when performing this command: " << exception.what () << std::endl;
            }
        };
    };
};
