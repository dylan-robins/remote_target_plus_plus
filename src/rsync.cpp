#include "rsync.hpp"

#include <filesystem>
namespace fs = std::filesystem;

#include "cpp-subprocess/subprocess.hpp"
namespace sp = subprocess;

#include <iostream>

static fs::path resolve_path(const fs::path& path) {
    char * home = std::getenv("HOME");

    std::string strPath = path.string();
    size_t tildeIndex;

    while ((tildeIndex = strPath.find('~') ) != std::string::npos) {
        if (home == NULL) {
            std::cerr << "Error: you have no HOME defined!\n";
            std::exit(1);
        }
        strPath.erase(tildeIndex);
        strPath.insert(tildeIndex, home);
    }

    return fs::weakly_canonical(strPath);
}

namespace rsync {

void copy(fs::path &source, fs::path &destination) {
    std::cout << "Source: " << resolve_path(source) << '\n'
              << "Target: " << resolve_path(destination) << '\n';
    // auto proc = sp::Popen(
    // {"rsync", "azn", source.c_str() , destination.c_str()},
    // sp::input{sp::PIPE},
    // sp::output{sp::PIPE});
    // proc.wait();
}

void ssh(fs::path &source, fs::path &destination, std::string &remote, std::string &username, std::string &password, fs::path &privateKey) {
    std::string target = "";
   
    if (username != "") {
        target += username;
        if (password != "") {
            target += ':' + password;
        }
        target += "@";
    }
    target += remote;
    if (destination.string()[0] != '/') {
        target += '/';
    }
    target += destination;

    std::cout << "Source: " << resolve_path(source) << '\n'
              << "Target: " << target << '\n';
    if (privateKey != "") {
        std::cout << "Private key: " << resolve_path(privateKey) << '\n';
    }
}
} // namespace rsync