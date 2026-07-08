#version 460 core

const vec2 quad[6] = vec2[](
    vec2(-1, -1), vec2(1, -1), vec2(1, 1),
    vec2(-1, -1), vec2(1, 1), vec2(-1, 1)
);

uniform mat4 view;
uniform mat4 projection;

out vec3 near_point;
out vec3 far_point;

vec3 unproject(float x, float y, float z)
{
    mat4 view_inv = inverse(view);
    mat4 proj_inv = inverse(projection);
    vec4 p = view_inv * proj_inv * vec4(x, y, z, 1.0);
    return p.xyz / p.w;
}

void main()
{
    vec2 p = quad[gl_VertexID];
    near_point = unproject(p.x, p.y, -1.0);
    far_point = unproject(p.x, p.y, 1.0);
    gl_Position = vec4(p, 0.0, 1.0);
}
