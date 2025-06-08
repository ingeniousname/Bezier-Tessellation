
#version 430 core
layout(vertices = 16) out;

uniform int subdivU, subdivV;

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if (gl_InvocationID == 0) {
        gl_TessLevelOuter[0] = subdivU;
        gl_TessLevelOuter[1] = subdivV;
        gl_TessLevelOuter[2] = subdivU;
        gl_TessLevelOuter[3] = subdivV;

        gl_TessLevelInner[0] = subdivV;
        gl_TessLevelInner[1] = subdivU;
    }
}