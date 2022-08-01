#include "config.hpp"
#include "yaml-cpp/yaml.h"

#include <filesystem>
#include <iostream>
#include <string>
namespace fs = std::filesystem;


void LocalConfig::print() const {
    std::cout << "LocalConfig: " << '\n'
              << "    remoteDir: " << remoteDir << '\n';
}
void LocalConfig::sync() const {
    std::cout << "rsync -azf " << sourceDir << remoteDir << " " << '\n';
}

void SSHConfig::print() const {
    std::cout << "SSHConfig: " << '\n'
              << "    remoteDir: " << remoteDir << '\n'
              << "    hostname: " << hostname << '\n'
              << "    username: " << username << '\n'
              << "    privateKey: " << privateKey << '\n'
              << "    password: " << password << '\n';
}
void SSHConfig::sync() const {
    std::cout << "rsync -azf " << sourceDir << remoteDir << " " << '\n';
}

static std::string getRequiredYAMLNodeValue(const YAML::Node &remote, const std::string key) {
    if (!remote[key]) {
        std::cerr << "Missing key '" << key << "' from remote config!\n";
        std::exit(1);
    }
    return remote[key].as<std::string>();
}
static std::string getOptionalYAMLNodeValue(const YAML::Node &remote, const std::string key) {
    return remote[key].as<std::string>("");
}

RemoteConfig RemoteConfigFactory(const fs::path &sourceDir, const YAML::Node &remote) {
    // Check remote type
    auto remoteType = getRequiredYAMLNodeValue(remote, "type");

    if (remoteType == "LOCAL") {
        LocalConfig conf{
            sourceDir,
            getRequiredYAMLNodeValue(remote, "remote_dir")};
        return conf;
    } else if (remoteType == "SSH") {
        SSHConfig conf{
            sourceDir,
            getRequiredYAMLNodeValue(remote, "remote_dir"),
            getRequiredYAMLNodeValue(remote, "hostname"),
            getOptionalYAMLNodeValue(remote, "username"),
            getOptionalYAMLNodeValue(remote, "privateKey"),
            getOptionalYAMLNodeValue(remote, "password")};
        return conf;
    } else {
        std::cerr << "Unknown remote type '" << remoteType << "'!\n";
        std::exit(1);
    }
}
