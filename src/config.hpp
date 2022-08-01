#pragma once
#ifndef __DIREPLICATE__CONFIIG_HPP__
#define __DIREPLICATE__CONFIIG_HPP__

#include <filesystem>
#include <iostream>
#include <variant>
#include <vector>
namespace fs = std::filesystem;

#include "yaml-cpp/yaml.h"

struct LocalConfig {
    fs::path remoteDir;

    void print() const {
        std::cout << "LocalConfig: " << '\n'
                  << "    remoteDir: " << remoteDir << '\n';
    }
};

struct SSHConfig {
    fs::path remoteDir;
    std::string hostname;
    std::string username;
    fs::path privateKey;
    std::string password;

    void print() const {
        std::cout << "SSHConfig: " << '\n'
                  << "    remoteDir: " << remoteDir << '\n'
                  << "    hostname: " << hostname << '\n'
                  << "    username: " << username << '\n'
                  << "    privateKey: " << privateKey << '\n'
                  << "    password: " << password << '\n';
    }
};

using RemoteConfig = std::variant<LocalConfig, SSHConfig>;
using RemoteConfigVector = std::vector<RemoteConfig>;

RemoteConfig RemoteConfigFactory(const YAML::Node &remote);

#endif