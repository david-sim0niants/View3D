#include "renderer.hpp"
#include "asset_manager.hpp"

#include <GL/glew.h>

namespace view3d {

Renderer::Renderer()
    : grid_shader(loadBuiltinShader("grid_vert.glsl", "grid_frag.glsl"))
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, &dummy_vao);
    // TODO: improve this
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &dummy_vao);
}

void Renderer::render(const Scene& scene)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGrid(*scene.getCamera());
}

void Renderer::renderGrid(const Camera& camera)
{
    grid_shader.use();
    grid_shader.setUniform("view", camera.getViewMatrix());
    grid_shader.setUniform("projection", camera.getProjectionMatrix());

    glBindVertexArray(dummy_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

} // namespace view3d
