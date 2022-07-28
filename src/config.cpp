#include "config.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
namespace fs = std::filesystem;

#include "inih/cpp/INIReader.h"

ConfigFile::ConfigFile(std::string &path) {
    _configPath = path;
    if (!fs::exists(_configPath)) {
        std::cerr << "Can't load config file '" << _configPath << "'\n";
        std::cerr << "File doesn't exist\n";
        std::exit(1);
    }
}

fs::path ConfigFile::rootDir() const {
    return _configPath.parent_path();
}

RemoteConfigVector ConfigFile::load() const {
    RemoteConfigVector configs;

    INIReader reader(_configPath);

    if (reader.ParseError() < 0) {
        std::cerr << "Can't load '" << _configPath << "'\n";
        return configs;
    }

    for (auto section : reader.GetSections()) {
        if (section == "LOCAL") {
            fs::path remoteDir = reader.Get("LOCAL", "RemoteDir", "");
            configs.push_back(
                std::shared_ptr<RemoteConfig>(new LocalConfig(remoteDir)));
        } else if (section == "SSH") {
            std::string hostname = reader.Get("SSH", "Hostname", "");
            std::string username = reader.Get("SSH", "Username", "");
            std::string password = reader.Get("SSH", "Password", "");
            fs::path privateKey = reader.Get("SSH", "PrivateKey", "");
            fs::path remoteDir = reader.Get("SSH", "RemoteDir", "");

            configs.push_back(
                std::shared_ptr<RemoteConfig>(new SSHConfig(remoteDir, hostname, username, privateKey, password)));
        }
    }
    return configs;
}