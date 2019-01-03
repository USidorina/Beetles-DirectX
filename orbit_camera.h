#pragma once
#ifndef ORBIT_CAMERA_H
#define ORBIT_CAMERA_H

#include <windows.h>
#include <directxmath.h>

using namespace DirectX;

class OrbitCamera {

public:

    OrbitCamera();

    void     initCamera(XMVECTOR & Eye, XMVECTOR & Up, XMVECTOR & At);
    XMMATRIX getViewMatrix();
    void     rotateCamera(float deltaX, float deltaY);
    void     zoomCamera(int zoomDelta);

    ~OrbitCamera() = default;

private:

    void initViewMatrix();

    static const float minZoomValue;
    static const float maxZoomValue;

    XMVECTOR eye;
    XMVECTOR at;
    XMVECTOR up;

    float curZoomValue;
    float curHorAngle;
    float curVertAngle;

    XMMATRIX viewMatrix;
};

#endif
