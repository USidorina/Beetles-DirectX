#include "body.h"


Body::Body() {
    
    //create 3 pairs of legs
    int childrenNumber = 3;

    for (int i = 0; i < childrenNumber; i++) {
        children.push_back(new Leg());
    }

    vertexBuffer     = nullptr;
    indexBuffer      = nullptr;
    constantBuffer   = nullptr;
}

void Body::init(ID3D11Device * g_pd3dDevice) {

    //init vertices and indices 
    initVertices();
    initIndices();

    //init vertex buffer for body
    vertexBuffer = Node::initVertexBuffer(g_pd3dDevice, vertices, verticesNumber);
    
    //init index buffer for body
    indexBuffer = Node::initIndexBuffer(g_pd3dDevice, indices, indicesNumber);

    //init the constant buffer
    constantBuffer = Node::initConstantBuffer(g_pd3dDevice);

    for (Node * child : children) {
        child->init(g_pd3dDevice);
    }
}

void Body::render(ID3D11DeviceContext * immediateContext, XMMATRIX & view, XMMATRIX & projection, XMMATRIX & world) {
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

    childrenMatrices.push_back(XMMatrixRotationZ(XM_PI / 12) *  XMMatrixRotationX(5.f * t) * XMMatrixTranslation(0.f, -0.8f, 0.f) * world);
    childrenMatrices.push_back(XMMatrixRotationZ(-XM_PI / 12) * XMMatrixRotationX(5.f * t) * XMMatrixTranslation(0.f, -0.8f, 0.15f) * world);
    childrenMatrices.push_back(XMMatrixRotationZ(-XM_PI / 12) * XMMatrixRotationX(5.f * t) * XMMatrixTranslation(0.f, -0.8f, -0.15f) * world);

    for (int i = 0; i < children.size(); i++) {
        children[i]->render(immediateContext, view, projection, childrenMatrices[i]);
    }
}

void Body::initVertices() {
    float stepPhi   = XM_2PI / circlesNumber;
    float stepTheta = XM_2PI / pointsInCircle;

    float x;
    float y;
    float z;

    float curTheta = 0.f;
    float curPhi   = 0.f;

    float scaleCoef  = 0.1f;
    float scaleCoefX = 1.1f;
    float scaleCoefY = 0.7f;
    float scaleCoefZ = 3.f;

    XMFLOAT4 color = XMFLOAT4(0.39f, 0.26f, 0.13f, 1.f);

    for (int i = 0; i < circlesNumber; i++, curPhi += stepPhi) {
        for (int j = 0; j < pointsInCircle; j++, curTheta += stepTheta) {
            x = XMScalarSin(curTheta) * XMScalarSin(curPhi) * scaleCoefX * scaleCoef;
            y = XMScalarCos(curTheta) * scaleCoefY * scaleCoef - 0.8f;
            z = XMScalarSin(curTheta) * XMScalarCos(curPhi) * scaleCoefZ * scaleCoef;
            vertices[i * pointsInCircle + j] = { XMFLOAT3(x, y, z), color };
        }
    }
}

void Body::initIndices() {
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

void Body::release() {

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

Body::~Body() {
    release();
}
