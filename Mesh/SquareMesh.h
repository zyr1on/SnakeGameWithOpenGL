#ifndef SQUARE_HPP
#define SQUARE_HPP

#include<glm/vec4.hpp>
#include<glm/vec3.hpp>
#include<cstdlib>

class Square {
public:
    enum DIRECTION {
        DIR_RIGHT =  1,
        DIR_LEFT  = -1,
        DIR_UP    =  2,
        DIR_DOWN  = -2
    };

    Square(float x,float y, float lenght) 
    {
        m_Position = glm::vec3(x,y,0.0f);
        m_Lenght = lenght;
        m_Color.r = (10+std::rand()%15)/25.0f;
        m_Color.g = (10+std::rand()%15)/25.0f;
        m_Direction = DIR_RIGHT; 
    }
    glm::vec3 getPosition() 
    {
        return m_Position;
    }
    glm::vec4 getColor() {
        return m_Color;
    }
    void move(float deltaTime = 1.0f, float multipler = 1.0f) 
    {
        switch (m_Direction)
        {
        case DIR_LEFT:
            m_Position -= glm::vec3(m_Lenght,0.0f,0.0f) * deltaTime * multipler;
            break;
        case DIR_RIGHT:
            m_Position += glm::vec3(m_Lenght,0.0f,0.0f) * deltaTime * multipler;
            break;
        case DIR_UP:
            m_Position += glm::vec3(0.0f,m_Lenght,0.0f) * deltaTime * multipler;
            break;
        case DIR_DOWN:
            m_Position -= glm::vec3(0.0f,m_Lenght,0.0f) * deltaTime * multipler;
            break;
        default:
            break;
        }
    }
    void setDirection(Square::DIRECTION dir) {
        m_Direction = dir;
    }
    void setColor(float r,float g,float b,float a) {
        m_Color = glm::vec4(r,g,b,a);
    }
    float getLength() {
        return m_Lenght;
    }
    DIRECTION getDirection() {
        return m_Direction;
    }
    
private:
    glm::vec4 m_Color;
    glm::vec3 m_Position;
    float m_Lenght;
    DIRECTION m_Direction;
};


#endif