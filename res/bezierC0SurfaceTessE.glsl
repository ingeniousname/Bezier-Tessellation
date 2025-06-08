#version 460 core
layout(quads, equal_spacing, ccw) in;
uniform mat4 projection;
uniform mat4 view;


vec3 interpolate1D(float t, vec3 p0, vec3 p1, vec3 p2, vec3 p3) {
    float it = 1.0 - t;
    return
        p0 * (it * it * it) +
        p1 * (3.0 * t * it * it) +
        p2 * (3.0 * t * t * it) +
        p3 * (t * t * t);
}

vec3 evaluateBezierSurface(float u, float v) {
    vec3 row[4];

    for (int i = 0; i < 4; ++i) {
        row[i] = interpolate1D(u,
            gl_in[i * 4 + 0].gl_Position.xyz,
            gl_in[i * 4 + 1].gl_Position.xyz,
            gl_in[i * 4 + 2].gl_Position.xyz,
            gl_in[i * 4 + 3].gl_Position.xyz);
    }

    return interpolate1D(v, row[0], row[1], row[2], row[3]);
}

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec3 position = evaluateBezierSurface(u, v);
    gl_Position = projection * view * vec4(position, 1.0);
}