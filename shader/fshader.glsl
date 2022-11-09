#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{    
    vec4 t = texture(image, TexCoords);
    /*if(t.a < 0.1)
        discard;*/
    color = spriteColor * t;
}  