#pragma once
std::map<unsigned int, unsigned int> textures;
ShaderMaterial* depthMat;
ShaderMaterial* rectMat;
ShaderMaterial* postProcessMat;
Rectangle* r;
Rectangle* postProcessRect;
FrameBuffer* fb;
FrameBuffer* postProcessFbo;
Texture* shadowMap;
Texture* postProcessTex;
PerspectiveCamera lightCamera;
void render(std::vector<IMesh*>& meshes, ICamera& camera);
void render(IMesh* mesh, ICamera& camera);
void updateUniforms(const IMaterial& material);
void renderToTexture(std::vector<IMesh*>& meshes, ICamera& camera);
void renderWithPostProcess(std::vector<IMesh*>& meshes, ICamera& camera);
unsigned int getTexId(const Texture * tex);
void postProcess(std::vector<IMesh*>& meshes, ICamera& camera);