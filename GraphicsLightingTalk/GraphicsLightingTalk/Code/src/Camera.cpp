#include "Camera.h"
#include <math.h>

void Camera::Setup(int _width, int _height, float _fovY, float _top, float _bottom, float _right, float _left, float _near, float _far, bool _freecam, bool _perspective)
{
    width = _width;
    height = _height;
    previousWidth = width;
    previousHeight = height;
    aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    fovY = _fovY;
    m_near = _near;
    m_far = _far;
    hasMoved = true;
    freeCam = _freecam;
    perspective = _perspective;
    top = _top;
    bottom = _bottom;
    right = _right;
    left = _left;
}

void Camera::Input(float deltaTime, GLFWwindow* window) {
    if (!freeCam)
        return;

    if (InputManager::GetKey(GLFW_KEY_UP)) {
        center += f * deltaTime * movementSpeed;
        eye += f * deltaTime * movementSpeed;
        hasMoved = true;
    }
    if (InputManager::GetKey(GLFW_KEY_DOWN)) {
        center -= f * deltaTime * movementSpeed;
        eye -= f * deltaTime * movementSpeed;
        hasMoved = true;
    }
    if (InputManager::GetKey(GLFW_KEY_RIGHT)) {
        center += r * deltaTime * movementSpeed;
        eye += r * deltaTime * movementSpeed;
        hasMoved = true;
    }
    if (InputManager::GetKey(GLFW_KEY_LEFT)) {
        center -= r * deltaTime * movementSpeed;
        eye -= r * deltaTime * movementSpeed;
        hasMoved = true;
    }
    if (InputManager::GetKey(GLFW_KEY_A))
    {
        center -= Vector3D(0.f, 1.f, 0.f) * deltaTime * movementSpeed;
        eye -= Vector3D(0.f, 1.f, 0.f) * deltaTime * movementSpeed;
        hasMoved = true;
    }
    if (InputManager::GetKey(GLFW_KEY_E))
    {
        center += Vector3D(0.f, 1.f, 0.f) * deltaTime * movementSpeed;
        eye += Vector3D(0.f, 1.f, 0.f) * deltaTime * movementSpeed;
        hasMoved = true;
    }

    if (InputManager::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (!hasClicked)
        {
            oldPos = InputManager::GetMousePosition();
            hasClicked = true;
        }
        Vector2D mousePos = InputManager::GetMousePosition();
        Vector2D deltaPos = mousePos - oldPos;
        oldPos = mousePos;
        mouseX = deltaPos.x;
        mouseY = deltaPos.y;

        float pitch = mouseY * sensitivity * deltaTime;
        float yaw = mouseX * sensitivity * deltaTime;

        worldPitch += pitch;
        worldYaw += yaw;
        
        if (worldPitch > 45.f)
            worldPitch = 45.f;
        else if (worldPitch < -45.f)
            worldPitch = -45.f;

        mat4x4 calculateX;
        mat4x4 calculateY;

        mat4x4 pitchRotationMatrix = calculateX.rotateX(worldPitch);
        mat4x4 yawRotationMatrix = calculateY.rotateY(worldYaw);
        mat4x4 finalRotationMatrix = pitchRotationMatrix * yawRotationMatrix;

        f = calculateX.transformDirection(finalRotationMatrix, Vector3D(0, 0, -1));

        center = eye + f;
        hasMoved = true;
    }

    else if (InputManager::IsMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT)) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        hasClicked = false;
    }
}

void Camera::Update(GLFWwindow* window)
{
    glfwGetFramebufferSize(window, &width, &height);
    if (width != previousWidth || height != previousHeight)
        hasMoved = true;
    previousWidth = width;
    previousHeight = height;
    if (!hasMoved)
        return;
    viewMatrix = getViewMatrix();
    projectionMatrix = getProjection();
    VP = projectionMatrix * viewMatrix;

    hasMoved = false;
}

mat4x4 Camera::getViewMatrix()
{
    f = f.normalize(center - eye);
    r = r.normalize(r.crossProduct(f, up));
    u = u.crossProduct(r, f);

    mat4x4 viewMatrix = {
    r.x, r.y, r.z, -r.dotProduct(eye),
    u.x, u.y, u.z, -u.dotProduct(eye),
    -f.x, -f.y, -f.z, f.dotProduct(eye),
    0.0f, 0.0f, 0.0f, 1.0f
    };
    return viewMatrix;;
}

mat4x4 Camera::getProjection()
{
    if (perspective)
    {
        mat4x4 perspective;
        float fovYrad = fovY * M_PI / 180;
        aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        perspective.PerspectiveMatrix(fovYrad, aspectRatio, m_near, m_far);
        return perspective;
    }
    mat4x4 orthographic;
    orthographic.OrthographicMatrix(top, bottom, right, left, m_near, m_far);
    return orthographic;
}

mat4x4 Camera::getVP()
{
    return VP; 
}
