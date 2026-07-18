#include <CLI/CLI.hpp>

#include "view3d.hpp"

namespace {

using namespace view3d;

void prepareScene(Scene& scene, const char* file_path)
{
    Camera* camera = createResource<Camera>(16.0f / 9.0f);
    Light* light = createResource<Light>(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    Mesh3D* mesh = createResource<Mesh3D>(loadMesh3D(file_path));
    Shader* shader = createResource<Shader>(
        loadBuiltinShader("basic_vertex.glsl", "basic_fragment.glsl"));
    Object* object = createResource<Object>(*mesh, *shader, glm::vec3{1.0f});

    scene.setCamera(camera);
    scene.setLight(light);
    scene.addObject(object);

    camera->translate(glm::vec3(0.0f, 20.0f, 0.0f));
    light->translate(glm::vec3(20.0f));
}

int run(const std::string& file_path)
{
    View view;
    prepareScene(view.getScene(), file_path.c_str());
    std::string title = "View3D - " + file_path;
    return view.view(title.c_str(), 1280, 720) ? 0 : 1;
}

} // namespace

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

    return run(file_path);
}
