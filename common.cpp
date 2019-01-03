#include "common.h"

ID3D11Buffer * Node::initVertexBuffer(ID3D11Device * g_pd3dDevice, SimpleVertex * vertices, int verticesNumber) {
    ID3D11Buffer * vertexBuffer;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * verticesNumber;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    g_pd3dDevice->CreateBuffer(&bd, &InitData, &vertexBuffer);
    return vertexBuffer;
}

ID3D11Buffer * Node::initIndexBuffer(ID3D11Device * g_pd3dDevice, WORD * indices, int indicesNumber) {
    ID3D11Buffer * indexBuffer;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * indicesNumber;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = indices;
    g_pd3dDevice->CreateBuffer(&bd, &InitData, &indexBuffer);
    return indexBuffer;
}

ID3D11Buffer * Node::initConstantBuffer(ID3D11Device * g_pd3dDevice) {
    ID3D11Buffer * constantBuffer;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    g_pd3dDevice->CreateBuffer(&bd, nullptr, &constantBuffer);
    return constantBuffer;
}