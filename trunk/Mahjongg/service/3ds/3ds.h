#pragma once

#include "Geometry.h"

// This is our face structure.  This is is used for indexing into the vertex
// and texture coordinate arrays.  From this information we know which vertices
// from our vertex array go to which face, along with the correct texture coordinates.

struct CFace
{
	int vertIndex[3];     // indicies for the verts that make up this triangle
	int coordIndex[3];      // indicies for the tex coords to texture this face
	CVector3 vFaceNormal;
	bool bSmooth;

	CFace();
};

// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them because you will want to eventually
// read in the UV tile ratio and the UV tile offset for some models.

struct CMaterialInfo
{
	char  strName[255];     // The texture name
	char  strFile[255];     // The texture file name (If this is set it's a texture map)
	BYTE  color[3];       // The color of the object (R, G, B)
	int   texureId;       // the texture ID
	float uTile;        // u tiling of texture  (Currently not used)
	float vTile;        // v tiling of texture  (Currently not used)
	float uOffset;          // u offset of texture  (Currently not used)
	float vOffset;        // v offset of texture  (Currently not used)

	CMaterialInfo();
};

// This holds all the information for our model/scene.
// You should eventually turn into a robust class that
// has loading/drawing/querying functions like:
// LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);

struct C3DObject
{
	int  numOfVerts;      // The number of verts in the model
	int  numOfFaces;      // The number of faces in the model
	int  numTexVertex;      // The number of texture coordinates
	int  materialID;      // The texture ID to use, which is the index into our texture array
	bool bHasTexture;     // This is TRUE if there is a texture map for this object
	char strName[255];      // The name of the object
	CVector3  *pVerts;      // The object's vertices
	CVector3  *pNormals;    // The object's normals
	CVector2  *pTexVerts;   // The texture's UV coordinates
	CFace *pFaces;        // The faces information of the object

	C3DObject();
};

// This holds our model information.  This should also turn into a robust class.
// We use STL's (Standard Template Library) vector class to ease our link list burdens. :)

struct C3DModel
{
	int numOfObjects;         // The number of objects in the model
	int numOfMaterials;         // The number of materials for the model
	CSimpleArray<CMaterialInfo> pMaterials; // The list of material information (Textures and colors)
	CSimpleArray<C3DObject> pObject;      // The object list for our model

	C3DModel();
};

// This class handles all of the loading code

class CLoad3DS
{
	//>------ Primary Chunk, at the beginning of each file
	enum CHUNK_ID
	{
		PRIMARY       = 0x4D4D,

		//>------ Main Chunks
		OBJECTINFO    = 0x3D3D,        // This gives the version of the mesh and is found right before the material and object information
		VERSION       = 0x0002,        // This gives the version of the .3ds file
		EDITKEYFRAME  = 0xB000,        // This is the header for all of the key frame info

		//>------ sub defines of OBJECTINFO
		MATERIAL    = 0xAFFF,        // This stored the texture info
		OBJECT      = 0x4000,        // This stores the faces, vertices, etc...

		//>------ sub defines of MATERIAL
		MATNAME       = 0xA000,        // This holds the material name
		MATDIFFUSE    = 0xA020,        // This holds the color of the object/material
		MATMAP        = 0xA200,        // This is a header for a new material
		MATMAPFILE    = 0xA300,        // This holds the file name of the texture

		OBJECT_MESH   = 0x4100,        // This lets us know that we are reading a new object

		//>------ sub defines of OBJECT_MESH
		OBJECT_VERTICES       = 0x4110,      // The objects vertices
		OBJECT_VERTEXOPTIONS  = 0x4111,      // The vertex options
		OBJECT_FACES          = 0x4120,      // The objects faces
		OBJECT_MATERIAL       = 0x4130,      // This is found if the object has a material, either texture map or color
		OBJECT_UV             = 0x4140,      // The UV texture coordinates
	};

	// This holds the chunk info

	struct tChunk
	{
		unsigned short int ID;          // The chunk's ID
		unsigned int length;          // The length of the chunk
		unsigned int bytesRead;         // The amount of bytes read within that chunk

		tChunk()
		{
			ID = 0;
			length = 0;
			bytesRead = 0;
		}
	};

	// Here is our structure for our 3DS indicies (since .3DS stores 4 unsigned shorts)

	struct tIndices
	{
		unsigned short a, b, c, bVisible;   // This will hold point1, 2, and 3 index's into the vertex array plus a visible flag

		tIndices()
		{
			a = b = c = bVisible = 0;
		}
	};

public:
	CLoad3DS();               // This inits the data members

	// This is the function that you call to load the 3DS
	bool Load(UINT nResourceID, C3DModel* pModel);
	bool Load(LPCTSTR szResourceName, C3DModel* pModel);

private:
	// This reads in a CString and saves it in the char array passed in
	int GetString(char *);

	// This reads the next chunk
	void ReadChunk(tChunk *);

	// This reads the next large chunk
	void ProcessNextChunk(C3DModel* pModel, tChunk *);

	// This reads the object chunks
	void ProcessNextObjectChunk(C3DModel *pModel, C3DObject *pObject, tChunk *);

	// This reads the material chunks
	void ProcessNextMaterialChunk(C3DModel *pModel, tChunk *);

	// This reads the RGB value for the object's color
	void ReadColorChunk(CMaterialInfo *pMaterial, tChunk *pChunk);

	// This reads the objects vertices
	void ReadVertices(C3DObject *pObject, tChunk *);

	//
	void ReadVertexOptions(C3DObject *pObject, tChunk *);

	// This reads the objects face information
	void ReadVertexIndices(C3DObject *pObject, tChunk *);

	// This reads the texture coodinates of the object
	void ReadUVCoordinates(C3DObject *pObject, tChunk *);

	// This reads in the material name assigned to the object and sets the materialID
	void ReadObjectMaterial(C3DModel *pModel, C3DObject *pObject, tChunk *pPreviousChunk);

	// This computes the vertex normals for the object (used for lighting)
	void ComputeNormals(C3DModel* pModel);

	// This frees memory and closes the file
	void CleanUp();

	//
	unsigned int Read(void* pBuffer, unsigned int nLength);

protected:
	// The data pointer
	LPBYTE m_pData;

	int buffer[50000];

	// These are used through the loading process to hold the chunk information
	tChunk* m_pCurrentChunk;
	tChunk* m_pTempChunk;
};
