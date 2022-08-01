#pragma once
#ifndef __YMLCONFIGFILE_HPP__
#define __YMLCONFIGFILE_HPP__

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

#include "config.hpp"


class ymlConfigFile {
    RemoteConfigVector _remotes;

  public:
    ymlConfigFile(const fs::path &configFile);

    void print() const;

    void runReplications(const fs::path &source_dir) const;
};

#endif