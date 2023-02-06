#version 330 core
out vec4 FragColor;

in vec2 texCoord;
uniform sampler2D gSampler;

void main()
{
    FragColor = texture2D(gSampler, texCoord);

    //FragColor = vec4(0.36, 0.25, 0.2, 1.0); // set alle 4 vector values to 1.0
}