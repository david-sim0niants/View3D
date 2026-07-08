#version 460 core

in vec3 frag_pos;
in vec3 frag_normal;

uniform vec3 light_position;
uniform vec3 light_color;
uniform float light_intensity;
uniform vec3 object_color;

out vec4 frag_color;

void main()
{
    vec3 norm = normalize(frag_normal);
    vec3 light_dir = normalize(light_position - frag_pos);

    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * light_color;

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color * light_intensity;

    vec3 result = (ambient + diffuse) * object_color;
    frag_color = vec4(result, 1.0);
}
