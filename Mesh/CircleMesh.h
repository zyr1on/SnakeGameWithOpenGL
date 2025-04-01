#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include<glad/glad.h>
#include<glm/vec4.hpp>
#include<glm/vec3.hpp>
#include<cmath>
#include<vector>

class Circle{
public:
    std::vector<float> vertices;
    Circle(float x, float y, float radius) {
        m_Radius = radius;
        segments = 100;
        m_Position = glm::vec3(x, y, 0.0f);
        for (int i = 0; i < segments; i++) {
            float angle = 2.0f * pi * float(i) / float(segments);
            float xCord = m_Radius * cos(angle);
            float yCord = m_Radius * sin(angle);
          
            vertices.push_back(x + xCord); 
            vertices.push_back(y + yCord); 
            vertices.push_back(0.0f);      
        }
    }
    void draw() {
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 3);
    }
    void setPosition(float x, float y) {
        m_Position = glm::vec3(x, y, 0.0f);
    }
    float getRadius() {
        return m_Radius;
    }
    void setPosition(glm::vec3& pos){
        m_Position = pos;
    }
    glm::vec3 getPosition() {
        return m_Position;
    }
    glm::vec4 getColor() {
        return m_Color;
    }
private:
    glm::vec3 m_Position;
    glm::vec4 m_Color;    
    int segments = 100;
    float pi =  3.14159;
    float m_Radius;
};
#endif
