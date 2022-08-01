#pragma once
#ifndef __DIREPLICATE__REMOTECONFIG_HPP__
#define __DIREPLICATE__REMOTECONFIG_HPP__

#include "tomlplusplus/toml.hpp"

#include <string>
#include <unordered_map>
#include <vector>

using stringMap = std::unordered_map<std::string, std::string>;


class remoteConfigs {
    std::vector<stringMap> _remotes;

  public:
    remoteConfigs(std::string &configFile);

    void print() const;
};

#endif