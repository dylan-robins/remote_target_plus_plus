#include "config.hpp"
#include "yaml-cpp/yaml.h"

#include <filesystem>
#include <iostream>
#include <string>
namespace fs = std::filesystem;


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

RemoteConfig RemoteConfigFactory(const YAML::Node &remote) {
    // Check remote type
    auto remoteType = getRequiredYAMLNodeValue(remote, "type");

    if (remoteType == "LOCAL") {
        LocalConfig conf{
            getRequiredYAMLNodeValue(remote, "remote_dir")};
        return conf;
    } else if (remoteType == "SSH") {
        SSHConfig conf{
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
