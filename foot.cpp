#include "foot.h"

Foot::Foot() {
    vertexBuffer   = nullptr;
    indexBuffer    = nullptr;
    constantBuffer = nullptr;
}

void Foot::init(ID3D11Device * g_pd3dDevice) {

    //init vertices and indices
    initVertices();
    initIndices();

    //init vertex buffer
    vertexBuffer = Node::initVertexBuffer(g_pd3dDevice, vertices, verticesNumber);

    //init index buffer
    indexBuffer = Node::initIndexBuffer(g_pd3dDevice, indices, indicesNumber);

    //init the constant buffer
    constantBuffer = Node::initConstantBuffer(g_pd3dDevice);

    //init children
    for (Node * child : children) {
        child->init(g_pd3dDevice);
    }
}

void Foot::render(ID3D11DeviceContext * immediateContext, XMMATRIX & view, XMMATRIX & projection, XMMATRIX & world) {

    ConstantBuffer cb;
    cb.mWorld      = XMMatrixTranspose(world);
    cb.mView       = XMMatrixTranspose(view);
    cb.mProjection = XMMatrixTranspose(projection);

    immediateContext->UpdateSubresource(constantBuffer, 0, nullptr, &cb, 0, 0);
    immediateContext->VSSetConstantBuffers(0, 1, &constantBuffer);

    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;

    immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    immediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
    immediateContext->DrawIndexed(indicesNumber, 0, 0);
}

void Foot::initVertices() {
    float stepPhi   = XM_2PI / circlesNumber;
    float stepTheta = XM_2PI / pointsInCircle;

    float x;
    float y;
    float z;

    float curTheta = 0.f;
    float curPhi   = 0.f;

    float scaleCoef  = 1.f / 15.f;
    float scaleCoefX = 0.6f;
    float scaleCoefY = 1.2f;
    float scaleCoefZ = 0.6f;

    XMFLOAT4 color = XMFLOAT4(0.39f, 0.26f, 0.13f, 1.f);

    for (int i = 0; i < circlesNumber; i++, curPhi += stepPhi) {
        for (int j = 0; j < pointsInCircle; j++, curTheta += stepTheta) {
            x = XMScalarCos(curTheta) * scaleCoefX * scaleCoef;
            y = XMScalarSin(curTheta) * XMScalarSin(curPhi) * scaleCoefY * scaleCoef;
            z = XMScalarSin(curTheta) * XMScalarCos(curPhi) * scaleCoefZ * scaleCoef;
            vertices[i * pointsInCircle + j] = { XMFLOAT3(x, y, z), color };
        }
    }
}

void Foot::initIndices() {
    for (WORD i = 0; i < circlesNumber; ++i) {
        for (WORD j = 0; j < pointsInCircle; ++j) {
            WORD currentBlock = (WORD)(6 * (pointsInCircle * i + j));
            indices[currentBlock] = (WORD)(pointsInCircle * i + j);
            indices[currentBlock + 1] = (WORD)(pointsInCircle * ((i + 1) % circlesNumber) + j);
            indices[currentBlock + 2] = (WORD)(pointsInCircle * ((i + 1) % circlesNumber) + ((j + 1) % pointsInCircle));
            indices[currentBlock + 3] = (WORD)(pointsInCircle * i + j);
            indices[currentBlock + 4] = (WORD)(pointsInCircle * ((i + 1) % circlesNumber) + ((j + 1) % pointsInCircle));
            indices[currentBlock + 5] = (WORD)(pointsInCircle * i + ((j + 1) % pointsInCircle));
        }
    }
}

void Foot::release() {

    for (Node * child : children) {
        child->release();
    }

    if (vertexBuffer)
        vertexBuffer->Release();
    if (indexBuffer)
        indexBuffer->Release();
    if (constantBuffer)
        constantBuffer->Release();
}

Foot::~Foot() {
    release();
}
