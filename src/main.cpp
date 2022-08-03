#include <filesystem>
#include <iostream>
#include <iterator>
namespace fs = std::filesystem;

#include "argparse/argparse.hpp"

#include "ymlConfigFile.hpp"

#ifndef PROJECT_NAME
#define PROJECT_NAME "remote_target"
#endif
#ifndef PROJECT_VERSION
#define PROJECT_VERSION "dev"
#endif

class ConfigurationNotFoundException : public std::exception {
  public:
    const char *what() {
        return "No remote_target configuration file was found in the current hierarchy!";
    }
};

fs::path find_config_in_parent(fs::path &starting_dir) {
    auto expected_file = starting_dir / ".remote_target";
    if (fs::is_regular_file(expected_file)) {
        return expected_file;
    } else {
        auto parent_dir = starting_dir.parent_path();
        if (parent_dir == starting_dir) {
            throw ConfigurationNotFoundException();
        }
        return find_config_in_parent(parent_dir);
    }
}

int main(int argc, const char *argv[]) {
    argparse::ArgumentParser program(PROJECT_NAME, PROJECT_VERSION);

    program.add_argument("--config").help("Path to a configuration file");
    program.add_argument("--dry-run")
        .default_value(false)
        .implicit_value(true)
        .help("If present, doesn't actually run the replication");

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    // Find the configuration file we want to read
    fs::path configFile;
    if (program.present("--config")) {
        configFile = program.get<std::string>("config");
    } else {
        auto cwd = fs::current_path();
        try {
            configFile = find_config_in_parent(cwd);
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
            std::exit(1);
        }
    }
    // Figure out if we want to run the replications or not
    bool dry_run = program["--dry-run"] == true;

    // Parse config file
    std::string pathToConfig = configFile.string();
    ymlConfigFile ymlConfigFile(pathToConfig);

    // Run the replication (or the dry-run)
    ymlConfigFile.runReplications(configFile.parent_path().string(), dry_run);

    return 0;
}