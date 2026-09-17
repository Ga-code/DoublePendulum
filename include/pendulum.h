#ifndef PENDULUM_H
#define PENDULUM_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/vector_double1_precision.hpp>
#include <shader.h>
const double g = 9.80665;
struct pendulum {
    glm::dvec2 Position1, Position2;
    glm::dvec2 Orgin;
    double Omega1, Omega2;
    double Theta1, Theta2;
    double Mass1, Mass2;
    double Length1, Length2;
    unsigned int VAO, VBO, VAOP, VBOP;

    shader Shader = shader("vertex.vs", "fragment.fs");
    glm::mat4 model = glm::mat4(1.0f);
    pendulum(glm::dvec2 Orgin, double Omega1, double Omega2,double Theta1, double Theta2, double Mass1, double Mass2,double Length1, double Length2){
        this->Orgin = Orgin;
        this->Omega1 = Omega1;
        this->Omega2 = Omega2;
        this->Theta1 = Theta1;
        this->Theta2 = Theta2;
        this->Mass1 = Mass1;
        this->Mass2 = Mass2;
        this->Length1 = Length1;
        this->Length2 = Length2;
        std::vector<float> vertices = {0.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 0.0f};
        glGenVertexArrays(1, &VAO);
        glGenVertexArrays(1, &VAOP);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        Shader.use();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(VAOP);
        glGenBuffers(1, &VBOP);
        glBindBuffer(GL_ARRAY_BUFFER, VBOP);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glm::mat4 projection = glm::ortho(-1920.0/1080.0, 1920.0/1080.0, -1.0, 1.0);
        Shader.setMatrix4("projection", projection); 

    }

    void draw(){
        Position1 = Length1*glm::dvec2(glm::sin(Theta1), -glm::cos(Theta1));
        Position2 = Position1 + Length2*glm::dvec2(glm::sin(Theta2), -glm::cos(Theta2));
        Shader.use();
        glPointSize(5.0f);
        glBindVertexArray(VAOP);
        std::vector<float> vertices = {
            (float)(Position1.x + Orgin.x), (float)(Position1.y + Orgin.y), 0.0f,
            (float)Orgin.x, (float)Orgin.y, 0.0f,
            (float)(Position2.x + Orgin.x), (float)(Position2.y + Orgin.y), 0.0f,
            (float)(Position1.x + Orgin.x), (float)(Position1.y + Orgin.y), 0.0f,
        };
        
        glBindBuffer(GL_ARRAY_BUFFER, VBOP);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINES, 0, 4);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        vertices = {
            (float)(Position1.x + Orgin.x), (float)(Position1.y + Orgin.y), 0.0f,
            (float)(Position2.x+ Orgin.x), (float)(Position2.y+ Orgin.y), 0.0f
        };
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, 2);
    }
    glm::dvec2 func(double t1, double t2, double w1, double w2){
        glm::dmat2x2 M = {Length1*Length1*(Mass1 + Mass2),
            Mass2*Length1*Length2*glm::cos(t1 - t2),
            Mass2*Length1*Length2*glm::cos(t1 - t2),
            Mass2*Length2*Length2};
        glm::dvec2 f = glm::dvec2( -Mass2*Length1*Length2*w2*w2*glm::sin(t1 - t2) - g*Length1*(Mass1 + Mass2)*glm::sin(t1), 
        -Mass2*g*Length2*glm::sin(t2) + Mass2*Length1*Length2*w1*w1*glm::sin(t1 - t2));

        M = glm::inverse(M);
        return M*f;
    }
    void step(double dTime){
        
        glm::dvec2 theta2prime = func(Theta1, Theta2, Omega1, Omega2);
        double k1v1 = theta2prime.x;
        double k1v2 = theta2prime.y;
        double k1x1 = Omega1;
        double k1x2 = Omega2;
        theta2prime = func(Theta1 + k1x1*dTime/2, Theta2 + k1x2*dTime/2, Omega1 + k1v1*dTime/2, Omega2 + k1v2*dTime/2);
        double k2v1 = theta2prime.x;
        double k2v2 = theta2prime.y;
        double k2x1 = Omega1 + k1v1*dTime/2;
        double k2x2 = Omega2 + k1v2*dTime/2;
        theta2prime = func(Theta1 + k2x1*dTime/2, Theta2 + k2x2*dTime/2, Omega1 + k2v1*dTime/2, Omega2 + k2v2*dTime/2);
        double k3v1 = theta2prime.x;
        double k3v2 = theta2prime.y;
        double k3x1 = Omega1 + k2v1*dTime/2;
        double k3x2 = Omega2 + k2v2*dTime/2;
        theta2prime = func(Theta1 + k3x1*dTime, Theta2 + k3x2*dTime, Omega1 + k3v1*dTime, Omega2 + k3v2*dTime);
        double k4v1 = theta2prime.x;
        double k4v2 = theta2prime.y;
        double k4x1 = Omega1 + k3v1*dTime;
        double k4x2 = Omega2 + k3v2*dTime;
        Theta1+=(k1x1 + 2.0*k2x1 + 2.0*k3x1 + k4x1)*dTime/6.0;
        Theta2+=(k1x2 + 2.0*k2x2 + 2.0*k3x2 + k4x2)*dTime/6.0;
        Omega1+=(k1v1 + 2.0*k2v1 + 2.0*k3v1 + k4v1)*dTime/6.0;
        Omega2+=(k1v2 + 2.0*k2v2 + 2.0*k3v2 + k4v2)*dTime/6.0;
        
    }
    void energy(){
        double KE = 0.5*Mass1*Length1*Length1*Omega1*Omega1 
          + 0.5*Mass2*(Length1*Length1*Omega1*Omega1 
          + Length2*Length2*Omega2*Omega2 
          + 2*Length1*Length2*Omega1*Omega2*cos(Theta1-Theta2));

        double PE = -(Mass1+Mass2)*g*Length1*cos(Theta1) 
           - Mass2*g*Length2*cos(Theta2);

        double E = KE + PE;
        std::cout <<"Energy: " << E << std::endl;
    }
};





#endif