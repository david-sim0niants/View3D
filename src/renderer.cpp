#include "renderer.hpp"
#include "asset_manager.hpp"

#include <GL/glew.h>

#include <format>

namespace view3d {

namespace {

void checkGLError(const char* msg)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        throw std::runtime_error(std::format("{} GL error: {}", msg, err));
}

void renderMesh(const Mesh3D& mesh)
{
    glBindBuffer(GL_ARRAY_BUFFER, mesh.getVertices().asDevice().getID());
    checkGLError("glBindBuffer");

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<3>),
                          (void*)0);
    checkGLError("glVertexAttribPointer");

    glEnableVertexAttribArray(0);
    checkGLError("glEnableVertexAttribArray");

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<3>),
                          (void*)offsetof(Vertex<3>, normal));
    checkGLError("glVertexAttribPointer");

    glEnableVertexAttribArray(1);
    checkGLError("glEnableVertexAttribArray");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getFaces().asDevice().getID());
    checkGLError("glBindBuffer");

    glDrawElements(GL_TRIANGLES, mesh.getFaces().size() * 3, GL_UNSIGNED_INT,
                   nullptr);
    checkGLError("glDrawElements");
}

std::unique_ptr<Mesh3D> uploadHostMesh(const Mesh3D& host_mesh)
{
    DeviceBuffer<Vertex<3>> device_vertices(host_mesh.getVertices().size());
    device_vertices.upload(host_mesh.getVertices().asHost());

    DeviceBuffer<Face<3>> device_faces(host_mesh.getFaces().size());
    device_faces.upload(host_mesh.getFaces().asHost());

    return std::make_unique<Mesh3D>(std::move(device_vertices),
                                    std::move(device_faces));
}

} // namespace

Renderer::Renderer()
    : grid_shader(loadBuiltinShader("grid_vert.glsl", "grid_frag.glsl"))
{
    glEnable(GL_BLEND);
    checkGLError("glEnable(GL_BLEND)");

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    checkGLError("glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)");

    glEnable(GL_DEPTH_TEST);
    checkGLError("glEnable(GL_DEPTH_TEST)");

    glGenVertexArrays(1, &dummy_vao);
    checkGLError("glGenVertexArrays");
    // TODO: improve this
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &dummy_vao);
    glGetError(); // ignore error
}

void Renderer::render(const Scene& scene)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    checkGLError("glClearColor");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    checkGLError("glClear");

    glBindVertexArray(dummy_vao);
    checkGLError("glBindVertexArray");

    renderGrid(*scene.getCamera());

    for (Object* obj : scene.getObjects())
        renderObject(scene, obj);
}

void Renderer::setViewport(int width, int height)
{
    glViewport(0, 0, width, height);
    checkGLError("glViewport");
}

void Renderer::renderGrid(const Camera& camera)
{
    grid_shader.setUniform("view", camera.getViewMatrix());
    grid_shader.setUniform("projection", camera.getProjectionMatrix());

    glDrawArrays(GL_TRIANGLES, 0, 6);
    checkGLError("glDrawArrays");
}

void Renderer::renderObject(const Scene& scene, const Object* object)
{
    const Camera& camera = *scene.getCamera();
    const Light& light = *scene.getLight();

    Shader shader = object->getShader();
    shader.setUniform("model", object->getMatrix());
    shader.setUniform("view", camera.getViewMatrix());
    shader.setUniform("projection", camera.getProjectionMatrix());

    shader.setUniform("light_position", light.getPosition());
    shader.setUniform("light_color", light.getColor());
    shader.setUniform("light_intensity", light.getIntensity());
    shader.setUniform("object_color", object->getColor());

    const Mesh3D* mesh = nullptr;
    auto it = dev_mesh_by_obj.find(object);
    if (it == dev_mesh_by_obj.end()) {
        mesh = object->getMesh();
        if (mesh->getVertices().inDevice())
            dev_mesh_by_obj[object] = mesh;
        else
            mesh = uploadAndAddMesh(object, mesh);
    } else {
        mesh = it->second;
    }

    renderMesh(*mesh);
}

const Mesh3D* Renderer::uploadAndAddMesh(const Object* object, const Mesh3D* mesh)
{
    auto device_mesh = uploadHostMesh(*mesh);

    Mesh3D* device_mesh_ptr = device_mesh.get();
    device_meshes.insert(std::move(device_mesh));

    return dev_mesh_by_obj[object] = device_mesh_ptr;
}

} // namespace view3d
