#pragma once
std::map<unsigned int, unsigned int> textures;
IMaterial*              depthMat;
IMaterial*              rectMat;
IMaterial*              postProcessMat;
IMaterial*              deferredShadingMat;
IMaterial*              deferredShadingRectMat[4];
Rectangle*                   r;
Rectangle*                   postProcessRect;
Rectangle*                   deferredShadingRect[4];
FrameBuffer*                 fb;
FrameBuffer*                 postProcessFbo;
FrameBuffer*                 deferredShadingFbo;
OpenGLResourceManager        resourceManager;
ITexture*                    shadowMap;
ITexture*                    postProcessTex;
ITexture*                    deferredShadingTex[3];
PerspectiveCamera            lightCamera;
Vec2                         resolution;
void initDeferredShading();
void initPostProcessing();
void initShadowMapping();
unsigned int depthRenderBuffer;
void render(std::vector<IMesh*>& meshes, ICamera& camera);
void render(IMesh* mesh, ICamera& camera);
void updateUniforms(const IMaterial& material);
void renderToTexture(std::vector<IMesh*>& meshes, ICamera& camera, Vec4& viewport = Vec4(0, 0, 0, 0));
void renderWithPostProcess(std::vector<IMesh*>& meshes, ICamera& camera);
void renderDeferred(std::vector<IMesh*>& meshes, ICamera& camera);
void postProcess(std::vector<IMesh*>& meshes, ICamera& camera);