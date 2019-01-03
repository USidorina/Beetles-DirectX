#include "leg.h"
#include "foot.h"

Leg::Leg() {

    //create 2 feet for every leg
    int childrenNumber = 2;

    for (int i = 0; i < childrenNumber; i++) {
        children.push_back(new Foot());
    }

    vertexBuffer = nullptr;
    indexBuffer = nullptr;
    constantBuffer = nullptr;
}

void Leg::init(ID3D11Device * g_pd3dDevice) {

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

void Leg::render(ID3D11DeviceContext * immediateContext, XMMATRIX & view, XMMATRIX & projection, XMMATRIX & world) {
    static float t = 0.0f;

    static ULONGLONG timeStart = 0;
    ULONGLONG timeCur = GetTickCount64();
    if (timeStart == 0)
    timeStart = timeCur;
    t = (timeCur - timeStart) / 1000.0f;

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

    std::vector<XMMATRIX> childrenMatrices;

    childrenMatrices.push_back(XMMatrixTranslation(-0.18f, -0.08f, 0.f) * XMMatrixRotationX(-5.f * t)  * world);
    childrenMatrices.push_back(XMMatrixTranslation(0.18f, -0.08f, 0.f) * XMMatrixRotationX(-5.f * t) * world);

    for (int i = 0; i < children.size(); i++) {
        children[i]->render(immediateContext, view, projection, childrenMatrices[i]);
    }
}

void Leg::initVertices() {
    float stepPhi   = XM_2PI / circlesNumber;
    float stepTheta = XM_2PI / pointsInCircle;

    float x;
    float y;
    float z;

    float curTheta = 0.f;
    float curPhi   = 0.f;

    float scaleCoef  = 1.f / 15.f;
    float scaleCoefX = 3.2f;
    float scaleCoefY = 0.8f;
    float scaleCoefZ = 0.8f;

    XMFLOAT4 color = XMFLOAT4(0.39f, 0.26f, 0.13f, 1.f);

    for (int i = 0; i < circlesNumber; i++, curPhi += stepPhi) {
        for (int j = 0; j < pointsInCircle; j++, curTheta += stepTheta) {
            x = XMScalarSin(curTheta) * XMScalarSin(curPhi) * scaleCoefX * scaleCoef;
            y = XMScalarCos(curTheta) * scaleCoefY * scaleCoef;
            z = XMScalarSin(curTheta) * XMScalarCos(curPhi) * scaleCoefZ * scaleCoef;
            vertices[i * pointsInCircle + j] = { XMFLOAT3(x, y, z), color };
        }
    }
}

void Leg::initIndices() {
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

void Leg::release() {

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

Leg::~Leg() {
    release();
}
