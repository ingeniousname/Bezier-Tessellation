#version 430 core

in vec3 te_normal;    // from TES, in view space
in vec3 te_position;  // from TES, in view space

out vec4 out_color;

uniform vec3 light_pos;     // in view space
uniform vec3 view_pos;      // usually (0,0,0) in view space
uniform vec4 color;         // base surface color

uniform float ambient_strength = 0.1;
uniform float specular_strength = 0.5;
uniform float shininess = 32.0;

void main()
{
    vec3 normal = normalize(te_normal);
    vec3 light_dir = normalize(light_pos - te_position);
    vec3 view_dir = normalize(view_pos - te_position);
    vec3 reflect_dir = reflect(-light_dir, normal);

    // Phong lighting components
    float diff = max(dot(normal, light_dir), 0.0);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);

    vec3 ambient = ambient_strength * color.rgb;
    vec3 diffuse = diff * color.rgb;
    vec3 specular = specular_strength * spec * vec3(1.0); // white specular highlights

    vec3 result = ambient + diffuse + specular;
    out_color = vec4(result, color.a);
}