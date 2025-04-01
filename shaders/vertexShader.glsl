#version 330 core

layout (location=0) in vec3 vertexPos;

uniform vec3 uMove;

void main(){
    gl_Position = vec4(vertexPos + uMove,1.0);
}
