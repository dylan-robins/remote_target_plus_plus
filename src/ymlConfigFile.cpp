#include "ymlConfigFile.hpp"

#include <iostream>
#include <string>
#include <filesystem>
#include <variant>
namespace fs = std::filesystem;

#include "yaml-cpp/yaml.h"
#include "config.hpp"

ymlConfigFile::ymlConfigFile(const fs::path &configFile) {
    YAML::Node config = YAML::LoadFile(configFile);

    // Check that the file is stuctures as a list of maps
    if (!config.IsSequence()) {
        std::cerr << "Incorrect file structure, should be a sequence of remotes!\n";
        std::exit(1);
    }

    for (auto remote : config) {
        // Check that each remote is defined as a map
        if (!remote.IsMap()) {
            std::cerr << "Incorrect file structure, remote definition should be a map!\n";
            std::exit(1);
        }
        _remotes.push_back(RemoteConfigFactory(remote));
    }
}

void ymlConfigFile::print() const {
    for (auto remote : _remotes) {
        auto* localConfig  = std::get_if<LocalConfig>(&remote);
        if (localConfig) {
            localConfig->print();
        }
        auto* sshConfig  = std::get_if<SSHConfig>(&remote);
        if (sshConfig) {
            sshConfig->print();
        }
    }
}

void ymlConfigFile::runReplications(const fs::path &source_dir) const {
    for (auto remote : _remotes) {
        auto* localConfig  = std::get_if<LocalConfig>(&remote);
        if (localConfig) {
            std::cout << "- Running rsync for Local config\n";
        }
        auto* sshConfig  = std::get_if<SSHConfig>(&remote);
        if (sshConfig) {
            std::cout << "- Running rsync for SSH config\n";
        }
    }
}
