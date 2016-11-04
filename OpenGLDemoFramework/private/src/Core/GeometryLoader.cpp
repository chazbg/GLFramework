#include "Core/GeometryLoader.hpp"
#include "Core/IResourceManager.hpp"
#include "Core/Mesh.hpp"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

static void loadMesh(IResourceManager& rm, std::shared_ptr<MeshNode> meshNode, const aiScene* scene, aiNode* node, aiMesh* mesh)
{
    auto bufferGeometry = meshNode->getMesh();
    Matrix4 modelMat(
        Vec4(node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4),
        Vec4(node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4),
        Vec4(node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4),
        Vec4(node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4)
        );

    meshNode->setModelMatrix(modelMat);
    if (mesh->HasPositions())
    {
        float* vertexBuffer = new float[3 * mesh->mNumVertices];

        for (unsigned int j = 0, k = 0; j < mesh->mNumVertices; j++, k += 3)
        {
            aiVector3D pos = mesh->mVertices[j];
            //printf("%f, %f, %f\n", pos.x, pos.y, pos.z);
            vertexBuffer[k] = pos.x;
            vertexBuffer[k + 1] = pos.y;
            vertexBuffer[k + 2] = pos.z;
        }

        IVertexBuffer* vertices = rm.createVertexBuffer(mesh->mNumVertices, 3, 0, vertexBuffer);
        delete[] vertexBuffer;
        bufferGeometry->setVertices(*vertices);
    }

    if (mesh->HasNormals())
    {
        float* normalsBuffer = new float[3 * mesh->mNumVertices];

        for (unsigned int j = 0, k = 0; j < mesh->mNumVertices; j++, k += 3)
        {
            aiVector3D normal = mesh->mNormals[j];
            //printf("normal %f, %f, %f\n", normal.x, normal.y, normal.z);
            normalsBuffer[k] = normal.x;
            normalsBuffer[k + 1] = normal.y;
            normalsBuffer[k + 2] = normal.z;
        }

        IVertexBuffer* normals = rm.createVertexBuffer(mesh->mNumVertices, 3, 1, normalsBuffer);
        delete[] normalsBuffer;
        bufferGeometry->setNormals(*normals);
    }

    if (mesh->HasTangentsAndBitangents())
    {
        float* tangentsBuffer = new float[3 * mesh->mNumVertices];
        float* bitangentsBuffer = new float[3 * mesh->mNumVertices];

        for (unsigned int j = 0, k = 0; j < mesh->mNumVertices; j++, k += 3)
        {
            aiVector3D tangent = mesh->mTangents[j];
            tangentsBuffer[k] = tangent.x;
            tangentsBuffer[k + 1] = tangent.y;
            tangentsBuffer[k + 2] = tangent.z;

            aiVector3D bitangent = mesh->mBitangents[j];
            bitangentsBuffer[k] = bitangent.x;
            bitangentsBuffer[k + 1] = bitangent.y;
            bitangentsBuffer[k + 2] = bitangent.z;
        }

        IVertexBuffer* tangents = rm.createVertexBuffer(mesh->mNumVertices, 3, 3, tangentsBuffer);
        IVertexBuffer* bitangents = rm.createVertexBuffer(mesh->mNumVertices, 3, 4, bitangentsBuffer);
        delete[] tangentsBuffer;
        delete[] bitangentsBuffer;

        bufferGeometry->setTangents(*tangents);
        bufferGeometry->setBitangents(*bitangents);
    }

    if (mesh->HasTextureCoords(0))
    {
        float* texCoordsBuffer = new float[2 * mesh->mNumVertices];

        for (unsigned int j = 0, k = 0; j < mesh->mNumVertices; j++, k += 2)
        {
            aiVector3D uv = mesh->mTextureCoords[0][j];
            //printf("uv %f, %f\n", uv.x, uv.y);
            texCoordsBuffer[k] = uv.x;
            texCoordsBuffer[k + 1] = uv.y;
        }

        IVertexBuffer* uvs = rm.createVertexBuffer(mesh->mNumVertices, 2, 2, texCoordsBuffer);
        delete[] texCoordsBuffer;
        bufferGeometry->setTexCoords(*uvs);
    }

    if (mesh->HasFaces())
    {
        unsigned int* indexBuffer = new unsigned int[3 * mesh->mNumFaces];

        for (unsigned int j = 0, k = 0; j < mesh->mNumFaces; j++, k += 3)
        {
            aiFace face = mesh->mFaces[j];
            //printf("%d, %d, %d\n", face.mIndices[0], face.mIndices[1], face.mIndices[2]);
            indexBuffer[k] = face.mIndices[0];
            indexBuffer[k + 1] = face.mIndices[1];
            indexBuffer[k + 2] = face.mIndices[2];
        }

        IIndexBuffer* indices = rm.createIndexBuffer(3 * mesh->mNumFaces, indexBuffer);
        delete[] indexBuffer;
        bufferGeometry->setIndices(*indices);
    }
}
static void loadChild(IResourceManager& rm, std::shared_ptr<MeshNode> parent, const aiScene* scene, aiNode* child)
{
    if (child->mNumMeshes > 0)
    {
        auto bufferGeometry = std::shared_ptr<Mesh>(new Mesh());
        auto meshNode = std::shared_ptr<MeshNode>(new MeshNode(bufferGeometry));

        // Our scene graph supports one mesh per node.
        // Interpret the first mesh as the parent mesh.
        // Interpret the rest of the meshes as children.
        // TODO: This interpretation is probably incorrect: 
        // Multiple meshes having transformations, relative to the same parent.
        loadMesh(rm, meshNode, scene, child, scene->mMeshes[child->mMeshes[0]]);

        for (unsigned int i = 1; i < child->mNumMeshes; i++)
        {
            auto childBufferGeometry = std::shared_ptr<Mesh>(new Mesh());
            auto childMeshNode = std::shared_ptr<MeshNode>(new MeshNode(childBufferGeometry));

            aiMesh* mesh = scene->mMeshes[child->mMeshes[i]];
            loadMesh(rm, childMeshNode, scene, child, mesh);

            childMeshNode->setParent(meshNode);
            meshNode->addChild(childMeshNode);
        }

        meshNode->setParent(parent);
        parent->addChild(meshNode);

        for (unsigned int i = 0; i < child->mNumChildren; i++)
        {
            loadChild(rm, meshNode, scene, child->mChildren[i]);
        }
    }    
}

std::shared_ptr<MeshNode> GeometryLoader::loadCustomGeometry(IResourceManager& rm, const std::string fileName)
{
    return loadCustomGeometry(rm, fileName, false);
}

std::shared_ptr<MeshNode> GeometryLoader::loadCustomGeometry(IResourceManager& rm, const std::string fileName, const bool flipFaces)
{
    Assimp::Importer importer;
    unsigned int flags =
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        aiProcess_CalcTangentSpace |
        aiProcess_OptimizeGraph | 
        aiProcess_OptimizeMeshes;

    if (flipFaces)
    {
        flags |= aiProcess_FlipWindingOrder;
    }

    const aiScene* scene = importer.ReadFile(fileName, flags);

    if (!scene)
    {
        printf("Assimp Scene Import Error: %s \n", importer.GetErrorString());
    }

    auto bufferGeometry = std::shared_ptr<Mesh>(new Mesh());
    auto rootNode = std::shared_ptr<MeshNode>(new MeshNode(bufferGeometry));

    aiNode* node = scene->mRootNode;
    unsigned int childrenStartIndex = 0;

    if (node->mNumMeshes > 0)
    {
        // Our scene graph supports one mesh per node.
        // Interpret the first mesh as the parent mesh.
        // Interpret the rest of the meshes as children.
        // TODO: This interpretation is probably incorrect: 
        // Multiple meshes having transformations, relative to the same parent.
        aiMesh* mesh = scene->mMeshes[node->mMeshes[0]];
        loadMesh(rm, rootNode, scene, node, mesh);

        for (unsigned int i = 1; i < node->mNumMeshes; i++)
        {
            auto childBufferGeometry = std::shared_ptr<Mesh>(new Mesh());
            auto childMeshNode = std::shared_ptr<MeshNode>(new MeshNode(childBufferGeometry));

            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            loadMesh(rm, childMeshNode, scene, node, mesh);

            childMeshNode->setParent(rootNode);
            rootNode->addChild(childMeshNode);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            loadChild(rm, rootNode, scene, node->mChildren[i]);
        }
    }

    return rootNode;
}
