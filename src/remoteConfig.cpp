#include "remoteConfig.hpp"

#include <iostream>
#include <string>

#include "tomlplusplus/toml.hpp"


static void getLocalConfig(const toml::table *remote, stringMap &map) {
    auto remote_dir = remote->get("remote_dir");
    if (remote_dir) map["remote_dir"] = remote_dir->value_or<std::string>("");
}

static void getSSHConfig(const toml::table *remote, stringMap &map) {
    auto hostname = remote->get("hostname");
    auto remote_dir = remote->get("remote_dir");
    auto username = remote->get("username");
    auto password = remote->get("password");
    auto private_key = remote->get("private_key");

    if (hostname) map["hostname"] = hostname->value_or<std::string>("");
    if (remote_dir) map["remote_dir"] = remote_dir->value_or<std::string>("");
    if (username) map["username"] = username->value_or<std::string>("");
    if (password) map["password"] = password->value_or<std::string>("");
    if (private_key) map["private_key"] = private_key->value_or<std::string>("");
}

remoteConfigs::remoteConfigs(std::string &configFile) {
    auto config = toml::parse_file(configFile).get_as<toml::array>("REMOTES");

    toml::array *parsed_remotes = config;

    for (auto &&remote : *parsed_remotes) {
        toml::table *tab = remote.as_table();
        if (!tab) {
            std::cerr << "Missing key 'type' in remote definition!\n";
            std::exit(1);
        }

        auto remote_type_node = tab->get("type");
        std::string remote_type;
        if (remote_type_node)
            remote_type = remote_type_node->value_or<std::string>("");
        else {
            std::cerr << "Missing key 'type' in remote definition!\n";
            std::exit(1);
        }
        stringMap map;
        if (remote_type == "LOCAL") {
            getLocalConfig(tab, map);
        } else if (remote_type == "SSH") {
            getSSHConfig(tab, map);
        }
        _remotes.push_back(map);
    }
}

void remoteConfigs::print() const {
    int i = 0;
    std::cout << "### Printing remotes\n";
    for (auto remote : _remotes) {
        std::cout << "### Remote " << i << '\n';
        //for (auto element : remote) {
        //    std::cout << element.first << ": " << element.second << std::endl;
        //}
        i++;
    }
}
