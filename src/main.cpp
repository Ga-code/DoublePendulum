#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext/vector_double1_precision.hpp>
#include <glm/gtx/string_cast.hpp>
#include <pendulum.h>

double lasttime = 0.0;
double dTime = 0.0;
int main(){
    if (!glfwInit()){
        std::cout << "too bad not working: GLFW";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Double Pendulum", NULL, NULL);
    if (window == NULL){
        std::cout << "no window";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "no glad";
        return -1;
    }
    glViewport(0,0, 1920, 1080);
    std::vector<pendulum> P;
    for (int i = 0; i<2; i++){
        P.emplace_back(glm::dvec2(0.0, 0.5), 0.0, 0.0, glm::radians(180.0f + i*0.00001f), glm::radians(90.0f), 1.0, 1.0, 0.5, 0.5);
    }
    //pendulum P(glm::dvec2(0.0), 0.0, 0.0,  glm::radians(95.0f), glm::radians(45.0f),  1.0, 1.0, 0.5, 0.5);
    lasttime = glfwGetTime();
    while (!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwTerminate();
          
            return 0;
        }
       
        double currenttime = glfwGetTime();
        dTime = currenttime - lasttime;
        lasttime = currenttime;
        int substeps = 1000;
        double subDt = dTime / substeps;
        for(int i = 0; i < substeps; i++){
            for (int j = 0; j<P.size(); j++){
                P.at(j).step(subDt);
            }
        }
        /*if (P.Theta >= glm::radians(89.99999)) {
            std::cout << glm::degrees(P.Theta) << std::endl;
        }
        
        //std::cout << glm::to_string(P.Position);
        //std::cout << glm::to_string(P.Velocity);
        */
        for (int i  = 0;i<P.size(); i++){
            P.at(i).draw();
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    
    return 0;
}