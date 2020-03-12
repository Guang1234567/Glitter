#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;

uniform float mixValue;

void main()
{
    //FragColor = vec4(ourColor, 1.0);
    //FragColor = texture(ourTexture0, TexCoord) * vec4(ourColor, 1.0);
    //FragColor = texture(ourTexture1, TexCoord) * vec4(ourColor, 1.0);
    FragColor = mix(texture(ourTexture0, TexCoord), texture(ourTexture1, TexCoord), 0.2);

    // revert x coordinate axis of texture
    // -----------------------------------
    // https://learnopengl.com/code_viewer.php?code=getting-started/textures-exercise1
    // 水平方向翻转纹理
    //FragColor = mix(texture(ourTexture0, TexCoord), texture(ourTexture1, vec2(1.0 - TexCoord.x, TexCoord.y)), mixValue);
}
