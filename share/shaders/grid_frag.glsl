#version 460 core

in vec3 near_point;
in vec3 far_point;

uniform mat4 view;
uniform mat4 projection;

out vec4 frag_color;

vec4 grid(vec3 frag_pos3d, float scale)
{
    vec2 coord = frag_pos3d.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid_lines = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid_lines.x, grid_lines.y);
    vec4 color = vec4(0.3, 0.3, 0.3, 1.0 - min(line, 1.0));
    return color;
}

float computeDepth(vec3 pos)
{
    vec4 clip = projection * view * vec4(pos, 1.0);
    return (clip.z / clip.w + 1.0) * 0.5;
}

void main()
{
    float t = -near_point.y / (far_point.y - near_point.y);
    if (t <= 0.0)
        discard;

    vec3 frag_pos3d = near_point + t * (far_point - near_point);

    vec4 color = grid(frag_pos3d, 1.0);
    if (color.a < 0.01)
        discard;

    gl_FragDepth = computeDepth(frag_pos3d);
    frag_color = color;
}
