#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;

void main()
{
    //FragColor = vec4(ourColor, 1.0);
    //FragColor = texture(ourTexture0, TexCoord) * vec4(ourColor, 1.0);
    //FragColor = texture(ourTexture1, TexCoord) * vec4(ourColor, 1.0);
    FragColor = mix(texture(ourTexture0, TexCoord), texture(ourTexture1, TexCoord), 0.2);
}
