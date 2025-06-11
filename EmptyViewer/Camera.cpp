#pragma once
#include "Camera.h"
#include "Ray.h"

using namespace glm;
using namespace std;


////Create Ray
//Ray* Camera::getRay(int ix, int iy) {
//    //coords of a pixel
//    float u = l + (r - l) * (ix + 0.5f) / 512;
//    float v = b + (t - b) * (iy + 0.5f) / 512;
//
//    vec3 p = e;
//    vec3 s = p + u * this->u + v * this->v - d * w;
//
//    Ray* ray = new Ray(p, s - p);
//
//    return ray;
//}
//
//Ray* Camera::getAntialiasingRay(int ix, int iy) {
//    float r1 = distribution(generator);
//    float r2 = distribution(generator);
//    float u = l + (r - l) * (ix + r1) / 512;
//
//    float v = b + (t - b) * (iy + r2) / 512;
//
//    vec3 p = e;
//    vec3 s = p + u * this->u + v * this->v - d * w;
//
//    Ray* ray = new Ray(p, s - p);
//
//    return ray;
//}

glm::mat4  Camera::getViewMatrix(){
    return glm::lookAt(e, e - w, v);
}

glm::mat4 Camera::getProjectionMatrix() {
    glm::mat4 proj(0.0f);
    proj[0][0] = 2 * nearPlane / (r - l);
    proj[1][1] = 2 * nearPlane / (t - b);
    proj[2][0] = (r + l) / (l - r);
    proj[2][1] = (t + b) / (b - t);
    proj[2][2] = (farPlane + nearPlane) / (nearPlane - farPlane);
    proj[2][3] = 1.0f;
    proj[3][2] = (2 * farPlane * nearPlane) / (farPlane - nearPlane);
    return proj;
}

//helped by chatGPT
bool Camera::isInFrustum(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) {
    // nearPlane, farPlane�� ������ ��, ��ȣ�� �ݴ�� ����ؾ� ��.
    float near = nearPlane < 0 ? farPlane : nearPlane;  // nearPlane�� ������ �� farPlane�� ������ �ٲٴ� ���
    float far = nearPlane < 0 ? nearPlane : farPlane;   // �ݴ�� farPlane�� nearPlane�� ������ ����

    // ī�޶��� near plane�� far plane�� ����Ͽ� �ﰢ���� z ���� ���� ���� �ִ��� Ȯ��
    if (v0.z < near || v0.z > far ||
        v1.z < near || v1.z > far ||
        v2.z < near || v2.z > far) {
        return false; // �ﰢ���� ī�޶��� ����� �Ǵ� ����� �ܿ� ����
    }

    return true;
}