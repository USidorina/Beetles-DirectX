#include "orbit_camera.h"


const float OrbitCamera::maxZoomValue = 4.f;
const float OrbitCamera::minZoomValue = 0.5f;

OrbitCamera::OrbitCamera() {

    curZoomValue = 1.f;
    curVertAngle = 0.f;
    curHorAngle  = 0.f;
}

void OrbitCamera::initCamera(XMVECTOR & Eye, XMVECTOR & Up, XMVECTOR & At) {

    this->eye = Eye;
    this->at  = At;
    this->up  = Up;

    initViewMatrix();
}

void OrbitCamera::initViewMatrix() {

    viewMatrix = XMMatrixLookAtLH(eye, at, up);
}

XMMATRIX OrbitCamera::getViewMatrix() {

    return viewMatrix;
}

void OrbitCamera::rotateCamera(float deltaX, float deltaY) {

    float horAngle  = deltaX * XM_2PI;
    float vertAngle = deltaY * XM_2PI;

    curHorAngle  += horAngle;
    curVertAngle += vertAngle;

    viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(eye.m128_f32[0], eye.m128_f32[1], eye.m128_f32[2]));
    viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixRotationY(horAngle));
    viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixRotationX(vertAngle));
    viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(-eye.m128_f32[0], -eye.m128_f32[1], -eye.m128_f32[2]));
}

void OrbitCamera::zoomCamera(int zoomDelta) {

    float zoomValue = zoomDelta > 0 ? 1.1f : 0.9f;
    curZoomValue *= zoomValue;

    if (curZoomValue < minZoomValue || curZoomValue > maxZoomValue) {
        return;
    }

    viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(eye.m128_f32[0], eye.m128_f32[1], eye.m128_f32[2]));
    viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixScaling(zoomValue, zoomValue, zoomValue));
    viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(-eye.m128_f32[0], -eye.m128_f32[1], -eye.m128_f32[2]));
}
