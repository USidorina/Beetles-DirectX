#pragma once 
#ifndef BODY_H
#define BODY_H

#include "common.h"
#include "leg.h"

class Body : public Node {

public:
    Body();

    void init(ID3D11Device * g_pd3dDevice);
    void render(ID3D11DeviceContext * immediateContext, XMMATRIX & view, XMMATRIX & projection, XMMATRIX & world);
    void release();

    ~Body();

private:
    std::vector<Node *> children;

    ID3D11Buffer * vertexBuffer;
    ID3D11Buffer * indexBuffer;
    ID3D11Buffer * constantBuffer;

    static const int circlesNumber  = 10;
    static const int pointsInCircle = 20;

    static const int verticesNumber = circlesNumber * pointsInCircle;
    static const int indicesNumber  = verticesNumber * 6;

    SimpleVertex vertices[verticesNumber];
    WORD indices[indicesNumber];


    void initVertices();
    void initIndices();
};

#endif