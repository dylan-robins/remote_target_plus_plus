#include <iostream>
#include <iterator>
#include <filesystem>
namespace fs = std::filesystem;

#include "rsync.hpp"
#include "argparse/argparse.hpp"

#include "config.hpp"

int main(int argc, const char *argv[]) {
    argparse::ArgumentParser program("direplicate", "1.0.0");

    program.add_argument("--config")
        .help("Path to a configuration file")
        .default_value(std::string{"examples/test.remote"});

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    std::string pathToConfig = program.get<std::string>("config");
    ConfigFile configFile(pathToConfig);
    fs::path sourceDir = configFile.rootDir();
    auto configurations = configFile.load();

    for (auto itr = configurations.begin(); itr != configurations.end(); ++itr) {
        std::shared_ptr<RemoteConfig> config = *itr;
        switch (config->type) {
        case RemoteConfig::RemoteConfigType::LOCAL:
            {
                std::cout << "PROCESSING LOCAL REPLICATION CONFIGURATION\n";
                std::shared_ptr<LocalConfig> localConfig = std::dynamic_pointer_cast<LocalConfig>(config);
                rsync::copy(sourceDir, localConfig->remoteDir);
            }

            break;

        case RemoteConfig::RemoteConfigType::SSH:
            {
                std::cout << "PROCESSING SSH REPLICATION CONFIGURATION\n";
                std::shared_ptr<SSHConfig> sshConfig = std::dynamic_pointer_cast<SSHConfig>(config);
                rsync::ssh(sourceDir, sshConfig->remoteDir, sshConfig->hostname, sshConfig->username, sshConfig->password, sshConfig->privateKey);
            }
            break;

        default:
            break;

        }
    }
    
    return 0;
}