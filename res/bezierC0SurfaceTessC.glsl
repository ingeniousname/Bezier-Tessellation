#version 430 core
layout(vertices = 16) out;

uniform int subdivOuter, subdivInner;

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if (gl_InvocationID == 0) {
        gl_TessLevelOuter[0] = subdivOuter;
        gl_TessLevelOuter[1] = subdivOuter;
        gl_TessLevelOuter[2] = subdivOuter;
        gl_TessLevelOuter[3] = subdivOuter;

        gl_TessLevelInner[0] = subdivInner;
        gl_TessLevelInner[1] = subdivInner;
    }
}