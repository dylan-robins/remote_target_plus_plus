#include <iostream>
#include <iterator>
#include <filesystem>
namespace fs = std::filesystem;

#include "argparse/argparse.hpp"

#include "ymlConfigFile.hpp"

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

    fs::path configFile(program.get<std::string>("config"));

    std::string pathToConfig = configFile.string();
    ymlConfigFile ymlConfigFile(pathToConfig);
    ymlConfigFile.runReplications(configFile.parent_path().string());
    ymlConfigFile.print();

    return 0;
}