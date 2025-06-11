#version 460 core
layout(quads, equal_spacing, ccw) in;

uniform mat4 projection;
uniform mat4 view;

out vec3 te_normal;
out vec3 te_position;

vec3 interpolate1D(float t, vec3 p0, vec3 p1, vec3 p2, vec3 p3) {
    float it = 1.0 - t;
    return
        p0 * (it * it * it) +
        p1 * (3.0 * t * it * it) +
        p2 * (3.0 * t * t * it) +
        p3 * (t * t * t);
}

vec3 interpolate1D_derivative(float t, vec3 p0, vec3 p1, vec3 p2, vec3 p3) {
    float it = 1.0 - t;
    return
        p0 * (-3.0 * it * it) +
        p1 * (3.0 * it * it - 6.0 * t * it) +
        p2 * (6.0 * t * it - 3.0 * t * t) +
        p3 * (3.0 * t * t);
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

vec3 evaluateNormal(float u, float v) {
    vec3 du_row[4];
    vec3 dv_col[4];

    // Derivative in u direction: for each row
    for (int i = 0; i < 4; ++i) {
        du_row[i] = interpolate1D_derivative(u,
            gl_in[i * 4 + 0].gl_Position.xyz,
            gl_in[i * 4 + 1].gl_Position.xyz,
            gl_in[i * 4 + 2].gl_Position.xyz,
            gl_in[i * 4 + 3].gl_Position.xyz);
    }

    // Derivative in v direction: for each column
    for (int i = 0; i < 4; ++i) {
        dv_col[i] = interpolate1D_derivative(v,
            gl_in[0 * 4 + i].gl_Position.xyz,
            gl_in[1 * 4 + i].gl_Position.xyz,
            gl_in[2 * 4 + i].gl_Position.xyz,
            gl_in[3 * 4 + i].gl_Position.xyz);
    }

    vec3 dpdu = interpolate1D(v, du_row[0], du_row[1], du_row[2], du_row[3]);
    vec3 dpdv = interpolate1D(u, dv_col[0], dv_col[1], dv_col[2], dv_col[3]);

    return normalize(cross(dpdu, dpdv));
}

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec3 position = evaluateBezierSurface(u, v);
    vec3 normal = evaluateNormal(u, v);

    te_position = position;
    te_normal = mat3(view) * normal; // transform to view space

    gl_Position = projection * view * vec4(position, 1.0);
}