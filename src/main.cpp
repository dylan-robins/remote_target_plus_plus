#include <iostream>
#include <iterator>
#include <filesystem>
namespace fs = std::filesystem;

#include "argparse/argparse.hpp"

#include "ymlConfigFile.hpp"

#ifndef PROJECT_NAME
#define PROJECT_NAME "remote_target"
#endif

int main(int argc, const char *argv[]) {
    argparse::ArgumentParser program(PROJECT_NAME, "1.0.0");

    program.add_argument("--config")
        .help("Path to a configuration file")
        .default_value(std::string{"examples/both.remote_target"});

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

    return 0;
}