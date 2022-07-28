#pragma once
#ifndef __DIREPLICATE__CONFIIG_HPP__
#define __DIREPLICATE__CONFIIG_HPP__

#include <filesystem>
#include <vector>
namespace fs = std::filesystem;

#include "inih/cpp/INIReader.h"

// Abstract base struct
struct RemoteConfig {
    enum RemoteConfigType { LOCAL,
                            SSH };
    fs::path remoteDir;
    RemoteConfigType type;
    RemoteConfig(RemoteConfigType type, fs::path remoteDir) : type(type), remoteDir(remoteDir) {}
    virtual ~RemoteConfig() {}
};

typedef std::vector<std::shared_ptr<RemoteConfig>> RemoteConfigVector;

struct LocalConfig : public RemoteConfig {
    LocalConfig(fs::path remoteDir) : RemoteConfig(RemoteConfigType::LOCAL, remoteDir) {}
};

struct SSHConfig : public RemoteConfig {
    std::string hostname;
    std::string username;
    fs::path privateKey;
    std::string password;

    SSHConfig(
        fs::path remoteDir,
        std::string hostname,
        std::string username,
        fs::path privateKey,
        std::string password) : RemoteConfig(RemoteConfigType::SSH, remoteDir),
                                hostname(hostname),
                                username(username),
                                privateKey(privateKey),
                                password(password) {}
};

class ConfigFile {
  private:
    fs::path _configPath;

  public:
    ConfigFile(std::string &path);
    fs::path rootDir() const;
    RemoteConfigVector load() const;
};
#endif