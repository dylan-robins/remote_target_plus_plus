#pragma once
#ifndef __DIREPLICATE__CONFIIG_HPP__
#define __DIREPLICATE__CONFIIG_HPP__

#include <filesystem>
#include <iostream>
#include <variant>
#include <vector>
namespace fs = std::filesystem;

#include "yaml-cpp/yaml.h"

struct RemoteConfig {
    virtual void print() const = 0;
    virtual void sync(const bool dry_run = false) const = 0;
};

struct LocalConfig : public RemoteConfig {
    fs::path sourceDir;
    fs::path remoteDir;

    LocalConfig(fs::path sourceDir, fs::path remoteDir)
        : sourceDir(sourceDir), remoteDir(remoteDir){};

    void print() const override;
    void sync(const bool dry_run = false) const override;
};

struct SSHConfig : public RemoteConfig {
    fs::path sourceDir;
    fs::path remoteDir;
    std::string hostname;
    std::string username;
    fs::path privateKey;

    SSHConfig(fs::path sourceDir, fs::path remoteDir, std::string hostname, std::string username, fs::path privateKey)
        : sourceDir(sourceDir), remoteDir(remoteDir), hostname(hostname), username(username), privateKey(privateKey){};

    void print() const override;
    void sync(const bool dry_run = false) const override;
};

using RemoteConfigVector = std::vector<std::shared_ptr<RemoteConfig>>;

std::shared_ptr<RemoteConfig> RemoteConfigFactory(const fs::path &sourceDir, const YAML::Node &remote);

#endif
