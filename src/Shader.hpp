#ifndef SHADER_H
#define SHADER_H

#include<map>
#include "config.hpp"

class Shader {
public:
    Shader() {
        m_ProgramId = glCreateProgram();
    }
    ~Shader() {
        glDeleteProgram(m_ProgramId);
        std::cout << "Deleted Program"<<std::endl;
    }
    void attachShader(const std::string& shaderPath,unsigned int shaderType) {
        unsigned int shaderId = make_module(shaderPath,shaderType);
        glAttachShader(m_ProgramId,shaderId);
        glDeleteShader(shaderId);
        std::cout << "Deleted Shader : "<< shaderType << " : " << shaderId << std::endl;
        
    }
    void link() {
        glLinkProgram(m_ProgramId);
        int isLinked;
        char log[512];
        glGetShaderiv(m_ProgramId,GL_LINK_STATUS,&isLinked);
        if(!isLinked)
        {
            glGetProgramInfoLog(m_ProgramId,512,0,log);
            std::cout<<"Program Linking Error:"<<std::endl<<log<<std::endl;
        }
    }
    void use() {
        glUseProgram(m_ProgramId);
    }
    void addUniform(const std::string& varName) {
        m_UniformMap[varName] =  glGetUniformLocation(m_ProgramId,varName.c_str());
    }
    void setFloat(const std::string& varName, float value) {
        glUniform1f(m_UniformMap[varName],value);
    }
    void setVec3(const std::string& varName, const glm::vec3& value) {
        glUniform3f(m_UniformMap[varName],value.x,value.y,value.z);
    }
    void setVec4(const std::string& varName, const glm::vec4& value) {
        glUniform4f(m_UniformMap[varName],value.r,value.g,value.b,value.a);
    }
    
private:

    unsigned int m_ProgramId;
    std::map<std::string, unsigned int> m_UniformMap;
};



#endif