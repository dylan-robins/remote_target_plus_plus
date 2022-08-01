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
    fs::path sourceDir;
    fs::path remoteDir;

    void print() const;
    void sync() const;
};

struct SSHConfig {
    fs::path sourceDir;
    fs::path remoteDir;
    std::string hostname;
    std::string username;
    fs::path privateKey;
    std::string password;

    void print() const;
    void sync() const;
};

using RemoteConfig = std::variant<LocalConfig, SSHConfig>;
using RemoteConfigVector = std::vector<RemoteConfig>;

RemoteConfig RemoteConfigFactory(const fs::path &sourceDir, const YAML::Node &remote);

#endif