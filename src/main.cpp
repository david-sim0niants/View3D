#include <CLI/CLI.hpp>

int main(int argc, char* argv[])
{
    CLI::App app{"View3D - A 3D model viewer application"};

    std::string file_path;
    app.add_option("file", file_path, "Path to the 3D model file")->required();

    if (argc < 2) {
        std::cout << app.help() << std::endl;
        return 0;
    }

    CLI11_PARSE(app, argc, argv);

    std::cout << "Viewing 3D model from file: " << file_path << std::endl;
    return 0;
}
