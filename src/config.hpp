#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<iostream>


#include<fstream>
#include<sstream>
#include<string>
#include<vector>

unsigned int make_module(const std::string& filePath,unsigned int module_type) 
{
    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    file.open(filePath);
    while (std::getline(file,line))
    {
        bufferedLines << line << "\n";
    }
    std::string shaderSource = bufferedLines.str();
    const char* shaderSrc = shaderSource.c_str();
    bufferedLines.str("");
    file.close();

    unsigned int shaderModule = glCreateShader(module_type);
    glShaderSource(shaderModule,1,&shaderSrc,nullptr);
    glCompileShader(shaderModule);

    int succes;
    glGetShaderiv(shaderModule,GL_COMPILE_STATUS,&succes);
    if(!succes){
        char errorlog[1024];
        glGetShaderInfoLog(shaderModule,1024,nullptr,errorlog);
        std::cout <<"Shader compile error:\n"<<errorlog << std::endl;

    }
    return shaderModule; 
}


unsigned int make_shader(const std::string& vertexFilePath,const std::string& fragmentFilePath) {
    std::vector<unsigned int> modules;
    modules.push_back(make_module(vertexFilePath,GL_VERTEX_SHADER));
    modules.push_back(make_module(fragmentFilePath,GL_FRAGMENT_SHADER));
    
    unsigned int shader = glCreateProgram();
    for(unsigned int shaderModule : modules) {
        glAttachShader(shader,shaderModule);
    }
    glLinkProgram(shader);

    int succes;
    glGetProgramiv(shader,GL_LINK_STATUS,&succes);
    if(!succes){
        char errorlog[1024];
        glGetProgramInfoLog(shader,1024,nullptr,errorlog);
        std::cout <<"Shader linking error:\n"<<errorlog << std::endl;
    }
    for(unsigned int shaderModule : modules) {
        glDeleteShader(shaderModule);
    }
    return shader;

}

