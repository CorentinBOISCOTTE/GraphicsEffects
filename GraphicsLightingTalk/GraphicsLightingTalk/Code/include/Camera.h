#pragma once
#define _USE_MATH_DEFINES

#include "Math/matrice.h"

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include "Debug.h"

class Camera
{
public:
    Camera() = default;
    Camera(int _width, int _height, float _fovY, float _top, float _bottom, float _right, float _left, float _near, float _far, bool _freecam, bool _perspective);
    ~Camera() = default;

    //Matrix
    mat4x4 viewMatrix = viewMatrix.IdentitiesMatrix();
    mat4x4 projectionMatrix = projectionMatrix.IdentitiesMatrix();
    mat4x4 VP = VP.IdentitiesMatrix();

    void Update(GLFWwindow* window);
    void Input(float deltaTime, GLFWwindow* window);
    mat4x4 getViewMatrix();
    mat4x4 getProjection();
    mat4x4 getVP(); 

    bool freeCam;
    bool perspective;

    //ProjectionMatrix
    Vector3D eye = { 0.f, 0.f, 3.0f };
    Vector3D center = { 0.f, 0.f, -2.f };
    Vector3D up = { 0.f, 1.f, 0.f };

    Vector3D f;
    Vector3D r;
    Vector3D u;

    float m_near;
    float m_far;
    float aspectRatio;
    float fovY;
    float top, bottom, right, left;
    int width, height, previousWidth, previousHeight;

    //Camera 
    Vector3D rotation = { 0, 0, 0 };
    Vector3D scale = { 1, 1, 1 };
    Vector3D position = { 0, 0, 0 };

    //Camera Move
    float movementSpeed = 20.f;
    float sensitivity = 5.f;

    double mouseX = 0.0;
    double mouseY = 0.0;
    float mouseDeltaX = 0.0f;
    float mouseDeltaY = 0.0f;

    bool hasMoved;
    bool hasClicked = false;

    Vector2D oldPos;

    float worldPitch = 0;
    float worldYaw = 0;
}; 