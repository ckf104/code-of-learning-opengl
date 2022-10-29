#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{    
    vec4 TexColor = texture(texture1, TexCoords);
    if(TexColor.a < 0.1){
        discard;
    }
    FragColor = TexColor;
    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}