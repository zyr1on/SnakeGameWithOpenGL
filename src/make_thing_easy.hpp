#pragma once
#include<glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include<iostream>


std::ostream& operator<<(std::ostream& os, glm::mat4& other) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            os << other[i][j] << " ";
        os << '\n'; // Alternatif olarak std::endl yerine '\n' kullan
    }
    return os;
}
std::ostream& operator<<(std::ostream& os, glm::vec3& other) {
    os << glm::to_string(other);
    return os;
    
}
