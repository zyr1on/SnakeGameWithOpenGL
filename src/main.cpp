#include "config.hpp"
#include "Shader.hpp"
#include <Mesh/SquareMesh.h>
#include<Mesh/CircleMesh.h>
#include<thread>
#include<chrono>

const int targetFPS = 12;
const int frameDelay = 1000 / targetFPS;

float lenght = 0.06;
float vertices[] = {
    -lenght/2,lenght/2,0.0f, // sol üst 0
    -lenght/2,-lenght/2,0.0f, // sol alt 1
    lenght/2, -lenght/2,0.0f, // sağ alt 2 
    lenght/2,lenght/2,0.0f   // sağ üst 3
};
unsigned int elements[] = {
    0,1,2,
    0,2,3
};

std::vector<Square*> snakeList;
void moveSnake() 
{
    for(auto next : snakeList) {
        next->move();
    }
    for(int i=snakeList.size()-1;i>0;i--) {
        snakeList[i]->setDirection(snakeList[i-1]->getDirection());
    }
}

void drawSnake(Shader& program){
    for(auto next : snakeList) {
        program.setVec3("uMove",next->getPosition());
        program.setVec4("uColor",next->getColor());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 6 indeks (2 üçgen)
    }
}

void addToSnakeTail() {
    int elementCount = snakeList.size();
    if(elementCount == 0) {
        snakeList.push_back(new Square(0.0f,0.0f,lenght));
    }
    else {
        Square* lastSquare = snakeList[elementCount-1];
        glm::vec3 pos = lastSquare->getPosition();
        Square::DIRECTION dir = lastSquare->getDirection();
        switch (dir)
        {
        case Square::DIR_RIGHT:
            pos -= glm::vec3(lenght,0.0,0.0);
            break;
        case Square::DIR_LEFT:
            pos += glm::vec3(lenght,0.0,0.0);
            break;
        case Square::DIR_UP:
            pos -= glm::vec3(0.0,lenght,0.0);
            break;
        case Square::DIR_DOWN:
            pos += glm::vec3(0.0f,lenght,0.0);
            break;
        default:
            break;
        }
        Square* newSquare = new Square(pos.x,pos.y,lenght);
        newSquare->setDirection(dir);
        snakeList.push_back(newSquare);
    }
}

void freeSnake() {
    for(Square* next : snakeList)
        delete next;
}

bool checkCollision(Square* head, Circle* apple) {
    float nearestX = std::max(head->getPosition().x, std::min(apple->getPosition().x, head->getPosition().x + lenght));
    float nearestY = std::max(head->getPosition().y, std::min(apple->getPosition().y, head->getPosition().y + lenght));
    float dx = apple->getPosition().x - nearestX;
    float dy = apple->getPosition().y - nearestY;
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= 0.024f;
}
bool checkCollision(Square* A, Square* B) {
    
    float A_left = A->getPosition().x - A->getLength() / 2;
    float A_right = A->getPosition().x + A->getLength() / 2;
    float A_top = A->getPosition().y + A->getLength() / 2;
    float A_bottom = A->getPosition().y - A->getLength() / 2;
    
    
    float B_left = B->getPosition().x - B->getLength() / 2;
    float B_right = B->getPosition().x + B->getLength() / 2;
    float B_top = B->getPosition().y + B->getLength() / 2;
    float B_bottom = B->getPosition().y - B->getLength() / 2;
    
    
    if (A_right > B_left && A_left < B_right && A_top > B_bottom && A_bottom < B_top) {
        return true;
    }
    
    return false;
}

bool checkSelfCollision(Square* head) {
    for (size_t i = 1; i < snakeList.size(); ++i) {
        Square* bodyPart = snakeList[i];
        if (checkCollision(head,bodyPart)) {
            return true;
        }
    }
    return false;
}

float randInRange(float min, float max) {
    return min + (static_cast<float>(std::rand()) / RAND_MAX) * (max - min);
}

bool isAppleOnSnake(const glm::vec3& applePos) {
    for (size_t i = 0; i < snakeList.size(); ++i) {
        Square* bodyPart = snakeList[i];
        if (bodyPart->getPosition() == applePos) {
            return true;
        }
    }
    return false; 
}

glm::vec3 generateRandomVec3() {
    glm::vec3 randomPos;
    do {
        float x = randInRange(-0.8f, 0.8f);
        float y = randInRange(-0.8f, 0.8f);
        randomPos = glm::vec3(x, y, 0.0f);
    } while (isAppleOnSnake(randomPos));
    return randomPos;
}

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods) 
{
    if(key == GLFW_KEY_ESCAPE)
        glfwTerminate();
    if(action == GLFW_PRESS) {
        if(snakeList.size() != 0) {
            Square* first = snakeList[0];
            if(key == GLFW_KEY_LEFT && first->getDirection() != Square::DIR_RIGHT) {
                first->setDirection(Square::DIR_LEFT);
            }
            if(key == GLFW_KEY_RIGHT && first->getDirection() != Square::DIR_LEFT) {
                first->setDirection(Square::DIR_RIGHT);
            }
            if(key == GLFW_KEY_UP && first->getDirection() != Square::DIR_DOWN){
                first->setDirection(Square::DIR_UP);
            }
            if(key == GLFW_KEY_DOWN && first->getDirection() != Square::DIR_UP) {
                first->setDirection(Square::DIR_DOWN);
            }    
        }
    }
}

int main() {
    GLFWwindow* window;
    if(!glfwInit()) 
    {
        std::cout << "GLFW Could'nt start" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window = glfwCreateWindow(750,750,"OpenGL",nullptr,nullptr);
    glfwSetKeyCallback(window,key_callback);
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))) {
        glfwTerminate();
        return -1;
    }

    Circle* apple = new Circle(0.0f, 0.0f, 0.023f);
    Square* head = new Square(0.0f,0.0f,lenght);
    
    head->setColor(0.0f,0.0f,0.0f,1.0f);
    snakeList.push_back(head);
    addToSnakeTail();
    addToSnakeTail();
    addToSnakeTail();
    
    Shader program;
    {
        program.attachShader("./shaders/vertexShader.glsl",GL_VERTEX_SHADER);
        program.attachShader("./shaders/fragmentShader.glsl",GL_FRAGMENT_SHADER);
        program.link();
        program.addUniform("uMove");
        program.addUniform("uColor");
    }

    unsigned int squareVAO,squareVBO,squareEBO;
    {   
        glGenBuffers(1,&squareVBO);
        glGenBuffers(1,&squareEBO);
        
        glGenVertexArrays(1,&squareVAO);
        glBindVertexArray(squareVAO);
        
        glBindBuffer(GL_ARRAY_BUFFER,squareVBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,squareEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements),elements,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,(void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }
    unsigned int CircleVAO,circleVBO; {
        glGenBuffers(1,&circleVBO);
        glGenVertexArrays(1,&CircleVAO);
        
        glBindVertexArray(CircleVAO);
        
        glBindBuffer(GL_ARRAY_BUFFER,circleVBO);
        glBufferData(GL_ARRAY_BUFFER,apple->vertices.size() * sizeof(float),&apple->vertices[0],GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,(void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    program.use();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    bool isAppleEaten = false;
    glm::vec3 applePos = glm::vec3(0.7f,0.7f,0.0f);
    apple->setPosition(applePos);
    while(!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        {
            glBindVertexArray(squareVAO);        
            drawSnake(program);
            moveSnake();
            glBindVertexArray(0);
        }
        {
            glBindVertexArray(CircleVAO);
            program.setVec4("uColor",glm::vec4(1.0f,0.0f,0.0f,1.0f));
            program.setVec3("uMove",applePos);
            apple->setPosition(applePos);
            apple->draw();
            glBindVertexArray(0);
        }
        if(checkSelfCollision(head)) 
        {
            std::cout << "game over" << "\n";
            int a;
            std::cin >> a;
        }
        if(checkCollision(head,apple)){
            isAppleEaten = true;
        }
        if(isAppleEaten == true) 
        {
            applePos = generateRandomVec3();
            addToSnakeTail();
            isAppleEaten = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay));
        glfwSwapBuffers(window);
        
    }
    snakeList.clear();
    apple->vertices.clear();
    freeSnake();
    delete apple;
    glDeleteBuffers(1,&circleVBO);
    glDeleteBuffers(1,&squareVBO);
    glDeleteVertexArrays(1,&CircleVAO);
    glDeleteVertexArrays(1,&squareVAO);
    glfwTerminate();
    
    int a;
    std::cin >> a;

    return 0;
}
    
