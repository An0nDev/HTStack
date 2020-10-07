#pragma once

#include "../AppConfigLoader/StaticAppConfig.hpp"
#include "../SSL/SSLSetupVars.hpp"

#include <vector>
#include <string>
#include <optional>

namespace HTStack {
    class ServerConfiguration {
    public:
        enum AppSetupType {
            INLINE,
            FILE
        };
        AppSetupType appSetupType;
        std::optional <std::vector <StaticAppConfig>> appStaticConfigs;
        std::optional <std::string> appConfigPath;
        bool autoStart;
        std::string ip;
        int port;
        int backlog;
        int clientThreadPoolSize;
        int maxRecvSize;
        bool sloppySocketRestart;
        int streamedResponseBufferSize;
        SSLSetupVars ssl;
        ServerConfiguration (
            AppSetupType appSetupType_,
            std::optional <std::vector <StaticAppConfig>> const & appStaticConfigs_,
            std::optional <std::string> const & appConfigPath,
            bool const & autoStart_,
            std::string const & ip_,
            int const & port_,
            int const & backlog_,
            int const & clientThreadPoolSize_,
            int const & maxRecvSize_,
            bool const & sloppySocketRestart_,
            int const & streamedResponseBufferSize_,
            SSLSetupVars const & ssl_
        );
    };
};
