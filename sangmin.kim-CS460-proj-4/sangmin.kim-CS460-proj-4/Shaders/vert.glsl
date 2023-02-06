#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 LocalPos0;

const int MAX_BONES = 200;

uniform mat4 gWVP;
uniform mat4 gBones[MAX_BONES];
uniform int transformIndex;
uniform float timeTicks;

mat4 Interpolation(mat4 currTransform, mat4 nextTransform, float timeTicksFloat);

layout(std430, binding = 0) buffer boneDatas
{
    int bones[];
};
layout(std430, binding = 1) buffer weightDatas
{
    float weights[];
};
layout(std430, binding = 2) buffer indexStartDatas
{
    int indexStart[];
};
layout(std430, binding = 3) buffer indexEndDatas
{
    int indexEnd[];
};

//layout(std430, binding = 4) buffer transforms_
//{
//    mat4 transforms[];
//};

mat4 Interpolation(mat4 currTransform, mat4 nextTransform, float timeTicksFloat)
{
    return currTransform + (nextTransform * timeTicksFloat);
}



void main()
{
    int startIndex = indexStart[gl_VertexID];
    int endIndex = indexEnd[gl_VertexID];

    mat4 boneTransform = gBones[bones[startIndex] + transformIndex] * weights[startIndex];

    for (int i = startIndex + 1; i < endIndex; ++i)
    {
        int boneId = bones[i];
        float weight = weights[i];

        boneTransform += gBones[boneId + transformIndex] * weight;
    }

    vec4 posL = boneTransform * vec4(position, 1.0);
    gl_Position = gWVP * posL;

    TexCoord0 = texCoord;
    Normal0 = normal;
    LocalPos0 = position;
}