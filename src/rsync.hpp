#pragma once
#ifndef __DIREPLICATE_RSYNC_HPP__
#define __DIREPLICATE_RSYNC_HPP__

#include <filesystem>
namespace fs = std::filesystem;

#include "cpp-subprocess/subprocess.hpp"
namespace sp = subprocess;

#include <iostream>

namespace rsync {
    void copy(fs::path &source, fs::path &destination);
    void ssh(fs::path &source, fs::path &destination, std::string &remote, std::string &username, std::string &password, fs::path &privateKey);
} // namespace rsync

#endif /* __DIREPLICATE_RSYNC_HPP__ */