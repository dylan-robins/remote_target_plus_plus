#include "config.hpp"
#include "yaml-cpp/yaml.h"

#include "pstreams/pstream.h"
using namespace redi;

#include <filesystem>
#include <iostream>
#include <string>
namespace fs = std::filesystem;

static fs::path resolve_path(const fs::path &path) {
    char *home = std::getenv("HOME");

    std::string strPath = path.string();
    size_t tildeIndex;

    while ((tildeIndex = strPath.find('~')) != std::string::npos) {
        if (home == NULL) {
            std::cerr << "Error: you have no HOME defined!\n";
            std::exit(1);
        }
        strPath.erase(tildeIndex, 1);
        strPath.insert(tildeIndex, home);
    }

    return fs::weakly_canonical(strPath);
}

static void stream_child_output(ipstream &child) {
    char buf[1024];
    std::streamsize n;
    bool finished[2] = {false, false};
    while (!finished[0] || !finished[1]) {
        if (!finished[0]) {
            while ((n = child.err().readsome(buf, sizeof(buf))) > 0)
                std::cerr.write(buf, n);
            if (child.eof()) {
                finished[0] = true;
                if (!finished[1])
                    child.clear();
            }
        }
        if (!finished[1]) {
            while ((n = child.out().readsome(buf, sizeof(buf))) > 0)
                std::cout.write(buf, n).flush();
            if (child.eof()) {
                finished[1] = true;
                if (!finished[0])
                    child.clear();
            }
        }
    }
}

void LocalConfig::print() const {
    std::cout << "### Local replication target: " << '\n'
              << "sourceDir: " << sourceDir << '\n'
              << "remoteDir: " << remoteDir << '\n';
}
void LocalConfig::sync() const {
    print();

    std::vector<std::string> cmd({"rsync",
                                  "-avzh",
                                  "--mkpath",
                                  resolve_path(sourceDir).string(),
                                  resolve_path(remoteDir).string()});

    std::cout << "### Command executed: ";
    for (auto part : cmd) {
        std::cout << part << " ";
    }
    std::cout << std::endl;

    const pstreams::pmode mode = pstreams::pstdout | pstreams::pstderr;
    ipstream child(cmd, mode);
    stream_child_output(child);
}

void SSHConfig::print() const {
    std::cout << "### SSHConfig: " << '\n'
              << "sourceDir: " << sourceDir << '\n'
              << "remoteDir: " << remoteDir << '\n'
              << "hostname: " << hostname << '\n'
              << "username: " << username << '\n'
              << "privateKey: " << privateKey << '\n';
}
void SSHConfig::sync() const {
    print();

    std::vector<std::string> cmd({"rsync", "-avzh", "--mkpath"});

    // If SSH private key is provided, insert it into the command
    if (privateKey.string() != "") {
        cmd.push_back("-e");
        cmd.push_back("ssh -i " + resolve_path(privateKey).string());
    }

    // Add source dir
    cmd.push_back(resolve_path(sourceDir).string());

    // If username is provided, use it
    std::string target;
    if (username != "") {
        target += username + "@";
    }
    // Add hostname and remote path
    target += hostname + ':' + remoteDir.string() + ' ';
    cmd.push_back(target);

    std::cout << "### Command executed: ";
    for (auto part : cmd) {
        std::cout << part << " ";
    }
    std::cout << std::endl;

    const pstreams::pmode mode = pstreams::pstdout | pstreams::pstderr;
    ipstream child(cmd, mode);
    stream_child_output(child);
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

std::shared_ptr<RemoteConfig> RemoteConfigFactory(const fs::path &sourceDir, const YAML::Node &remote) {
    // Check remote type
    auto remoteType = getRequiredYAMLNodeValue(remote, "type");

    if (remoteType == "LOCAL") {
        auto conf = std::make_shared<LocalConfig>(sourceDir, getRequiredYAMLNodeValue(remote, "remote_dir"));
        return conf;
    } else if (remoteType == "SSH") {
        auto conf = std::make_shared<SSHConfig>(
            sourceDir,
            getRequiredYAMLNodeValue(remote, "remote_dir"),
            getRequiredYAMLNodeValue(remote, "hostname"),
            getOptionalYAMLNodeValue(remote, "username"),
            getOptionalYAMLNodeValue(remote, "private_key"));
        return conf;
    } else {
        std::cerr << "Unknown remote type '" << remoteType << "'!\n";
        std::exit(1);
    }
}
