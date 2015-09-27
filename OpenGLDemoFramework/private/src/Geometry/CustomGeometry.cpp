#include <Geometry/CustomGeometry.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

CustomGeometry::CustomGeometry(const std::string fileName)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if (!scene)
    {
        printf("Assimp Scene Import Error: %s \n", importer.GetErrorString());
    }

    aiNode* node = scene->mRootNode;
   
    for (int i = 0; i < 1; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        unsigned int* indexBuffer = new unsigned int[3 * mesh->mNumFaces];
        float* vertexBuffer = new float[3 * mesh->mNumVertices];
        float* normalsBuffer = new float[3 * mesh->mNumVertices];

        for (int j = 0, k = 0; j < mesh->mNumVertices; j++, k += 3)
        {
            aiVector3D pos = mesh->mVertices[j];
            printf("%f, %f, %f\n", pos.x, pos.y, pos.z);
            vertexBuffer[k] = pos.x;
            vertexBuffer[k + 1] = pos.y;
            vertexBuffer[k + 2] = pos.z;
        }

        for (int j = 0, k = 0; j < mesh->mNumVertices; j++, k += 3)
        {
            aiVector3D normal = mesh->mNormals[j];
            printf("normal %f, %f, %f\n", normal.x, normal.y, normal.z);
            normalsBuffer[k] = normal.x;
            normalsBuffer[k + 1] = normal.y;
            normalsBuffer[k + 2] = normal.z;
        }
        
        for (int j = 0, k = 0; j < mesh->mNumFaces; j++, k += 3)
        {
            aiFace face = mesh->mFaces[j];
            printf("%d, %d, %d\n", face.mIndices[0], face.mIndices[1], face.mIndices[2]);
            indexBuffer[k] = face.mIndices[0];
            indexBuffer[k + 1] = face.mIndices[1];
            indexBuffer[k + 2] = face.mIndices[2];
        }

        indices = new IndexBufferObject(indexBuffer, 3 * mesh->mNumFaces);
        vertices = new VertexBufferObject(vertexBuffer, mesh->mNumVertices, 3);
        normals = new VertexBufferObject(normalsBuffer, mesh->mNumVertices, 3);

        setIndices(*indices);
        setVertices(*vertices);
        setNormals(*normals);

        delete[] indexBuffer;
        delete[] vertexBuffer;
        delete[] normalsBuffer;
    }
}

CustomGeometry::~CustomGeometry()
{
    delete indices;
    delete vertices;
    delete normals;
}
