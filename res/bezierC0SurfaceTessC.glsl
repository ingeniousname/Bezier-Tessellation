#version 430 core
layout(vertices = 16) out;

uniform float subdivOuter, subdivInner;
uniform mat4 lod;

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if (gl_InvocationID == 0) {
        int gridWidth = 4;
        int patchID = gl_PrimitiveID;

        int row = patchID / gridWidth;
        int col = patchID % gridWidth;

        float centerLOD = lod[row][col];

        float topLOD = (row > 0) ? max(centerLOD, lod[row - 1][col]) : centerLOD;
        float rightLOD = (col < gridWidth - 1) ? max(centerLOD, lod[row][col + 1]) : centerLOD;
        float bottomLOD = (row < gridWidth - 1) ? max(centerLOD, lod[row + 1][col]) : centerLOD;
        float leftLOD = (col > 0) ? max(centerLOD, lod[row][col - 1]) : centerLOD;

        gl_TessLevelOuter[0] = int(clamp(subdivInner * topLOD, 1.f, 64.f));
        gl_TessLevelOuter[1] = int(clamp(subdivInner * rightLOD, 1.f, 64.f));
        gl_TessLevelOuter[2] = int(clamp(subdivInner * bottomLOD, 1.f, 64.f));
        gl_TessLevelOuter[3] = int(clamp(subdivInner * leftLOD, 1.f, 64.f));

        gl_TessLevelInner[0] = int(clamp(subdivOuter * centerLOD, 1.f, 64.f));
        gl_TessLevelInner[1] = int(clamp(subdivOuter * centerLOD, 1.f, 64.f));
    }
}