#include "ymlConfigFile.hpp"

#include <iostream>
#include <string>
#include <filesystem>
#include <variant>
namespace fs = std::filesystem;

#include "yaml-cpp/yaml.h"
#include "config.hpp"

ymlConfigFile::ymlConfigFile(const fs::path &configFile) : _configPath(configFile) {
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
        _remotes.push_back(RemoteConfigFactory(_configPath.parent_path(), remote));
    }
}

void ymlConfigFile::print() const {
    for (auto remote : _remotes) {
        remote->print();
    }
}

void ymlConfigFile::runReplications(const fs::path &source_dir, const bool dry_run) const {
    for (auto remote : _remotes) {
        remote->sync(dry_run);
    }
}
