#pragma once
std::map<unsigned int, unsigned int> textures;
ShaderMaterial* depthMat;
ShaderMaterial* rectMat;
Rectangle* r;
FrameBuffer* fb;
Texture* shadowMap;
PerspectiveCamera lightCamera;
void updateUniforms(const IMaterial& material);
void renderToTexture(std::vector<IMesh*>& meshes, ICamera& camera);