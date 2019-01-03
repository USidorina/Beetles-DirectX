#pragma once
#ifndef TUTORIAL05_H
#define TUTORIAL05_H

#include <DirectXMath.h>
#include <d3d11.h>
#include <windows.h>
#include <vector>
#include <stdlib.h>

using namespace DirectX;

struct SimpleVertex
{
    DirectX::XMFLOAT3 Pos;
    DirectX::XMFLOAT4 Color;
};

struct ConstantBuffer
{
    DirectX::XMMATRIX mWorld;
    DirectX::XMMATRIX mView;
    DirectX::XMMATRIX mProjection;
};

class Node {

public:
    virtual void init(ID3D11Device * g_pd3dDevice) = 0;
    virtual void render(ID3D11DeviceContext * g_pImmediateContext, XMMATRIX & view, XMMATRIX & projection, XMMATRIX & world) = 0;
    virtual void release() = 0;
    
    static ID3D11Buffer * initVertexBuffer(ID3D11Device * g_pd3dDevice, SimpleVertex * vertices, int verticesNumber);
    static ID3D11Buffer * initIndexBuffer(ID3D11Device * g_pd3dDevice, WORD * indices, int indicesNumber);
    static ID3D11Buffer * initConstantBuffer(ID3D11Device * g_pd3dDevice);
};

#endif
