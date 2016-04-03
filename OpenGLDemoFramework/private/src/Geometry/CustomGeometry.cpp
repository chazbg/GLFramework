#include <Geometry/CustomGeometry.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <stack>

CustomGeometry::CustomGeometry(const std::string fileName) : CustomGeometry(fileName, false)
{
}

CustomGeometry::CustomGeometry(const std::string fileName, const bool flipFaces)
{
    Assimp::Importer importer;
    unsigned int flags = 
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        aiProcess_CalcTangentSpace;

    if (flipFaces)
    {
        flags |= aiProcess_FlipWindingOrder;
    }

    const aiScene* scene = importer.ReadFile(fileName, flags);
	
	std::stack<aiNode*> nodes;

    if (!scene)
    {
        printf("Assimp Scene Import Error: %s \n", importer.GetErrorString());
    }

	nodes.push(scene->mRootNode);

	while (!nodes.empty())
	{
		aiNode* node = nodes.top();
		nodes.pop();

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			Mesh* bufferGeometry = new Mesh();

			Matrix4 modelMat(
				Vec4(node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4),
				Vec4(node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4),
				Vec4(node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4),
				Vec4(node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4)
				);

			bufferGeometry->setModelMatrix(modelMat);
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

				VertexBufferObject* vertices = new VertexBufferObject(vertexBuffer, mesh->mNumVertices, 3);
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

				VertexBufferObject* normals = new VertexBufferObject(normalsBuffer, mesh->mNumVertices, 3);
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

				VertexBufferObject* tangents = new VertexBufferObject(tangentsBuffer, mesh->mNumVertices, 3);
				VertexBufferObject* bitangents = new VertexBufferObject(bitangentsBuffer, mesh->mNumVertices, 3);
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

				VertexBufferObject* uvs = new VertexBufferObject(texCoordsBuffer, mesh->mNumVertices, 2);
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

				IndexBufferObject* indices = new IndexBufferObject(indexBuffer, 3 * mesh->mNumFaces);
				delete[] indexBuffer;
				bufferGeometry->setIndices(*indices);
			}

			addChild(bufferGeometry);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			nodes.push(node->mChildren[i]);
		}
	}
}

CustomGeometry::~CustomGeometry()
{

}
