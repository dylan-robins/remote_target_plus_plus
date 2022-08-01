#include <iostream>
#include <iterator>
#include <filesystem>
namespace fs = std::filesystem;

#include "rsync.hpp"
#include "argparse/argparse.hpp"

#include "remoteConfig.hpp"

int main(int argc, const char *argv[]) {
    argparse::ArgumentParser program("direplicate", "1.0.0");

    program.add_argument("--config")
        .help("Path to a configuration file")
        .default_value(std::string{"examples/both_key.remote"});

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    std::string pathToConfig = program.get<std::string>("config");
    remoteConfigs remoteConfigs(pathToConfig);
    remoteConfigs.print();

    return 0;
}