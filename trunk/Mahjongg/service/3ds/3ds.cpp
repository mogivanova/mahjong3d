#include "stdafx.h"
#include "3ds.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
CFace::CFace()
{
	vertIndex[0] = vertIndex[1] = vertIndex[2] = 0;
	coordIndex[0] = coordIndex[1] = coordIndex[2] = 0;
	bSmooth = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
C3DObject::C3DObject()
{
	numOfVerts = 0;      // The number of verts in the model
	numOfFaces = 0;      // The number of faces in the model
	numTexVertex = 0;      // The number of texture coordinates
	materialID = 0;      // The texture ID to use, which is the index into our texture array
	bHasTexture = 0;     // This is TRUE if there is a texture map for this object
	pVerts = NULL;      // The object's vertices
	pNormals = NULL;    // The object's normals
	pTexVerts = NULL;   // The texture's UV coordinates
	pFaces = NULL;        // The faces information of the object
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
CMaterialInfo::CMaterialInfo()
{
	color[0] = color[1] = color[2] = 0;       // The color of the object (R, G, B)
	texureId = 0;       // the texture ID
	uTile = 0.0f;        // u tiling of texture  (Currently not used)
	vTile = 0.0f;        // v tiling of texture  (Currently not used)
	uOffset = 0.0f;          // u offset of texture  (Currently not used)
	vOffset = 0.0f;        // v offset of texture  (Currently not used)
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
C3DModel::C3DModel()
{
	numOfObjects = 0;         // The number of objects in the model
	numOfMaterials = 0;         // The number of materials for the model
}


// This file handles all of the code needed to load a .3DS file.
// Basically, how it works is, you load a chunk, then you check
// the chunk ID.  Depending on the chunk ID, you load the information
// that is stored in that chunk.  If you do not want to read that information,
// you read past it.  You know how many bytes to read past the chunk because
// every chunk stores the length in bytes of that chunk.

///////////////////////////////////////////////////////////////////////////////////////////////////
// This constructor initializes the tChunk data
CLoad3DS::CLoad3DS()
{
	m_pCurrentChunk = new tChunk;        // Initialize and allocate our current chunk
	m_pTempChunk = new tChunk;         // Initialize and allocate a temporary chunk
}

///////////////////////////////// IMPORT 3DS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
///// This is called by the client to open the .3ds file, read it, then clean up
/////
///////////////////////////////// IMPORT 3DS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool CLoad3DS::Load(UINT nResourceID, C3DModel* pModel)
{
	return Load(MAKEINTRESOURCE(nResourceID), pModel);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CLoad3DS::Load(LPCTSTR szResourceName, C3DModel* pModel)
{
	HRSRC hRsrc = FindResource(_Module.m_hInstResource, szResourceName, _T("OBJECT3D"));

	if (hRsrc == NULL)
		return false;

	DWORD dwSize = SizeofResource(_Module.m_hInstResource, hRsrc);

	HGLOBAL hGlob = LoadResource(_Module.m_hInstResource, hRsrc);

	if (hGlob == NULL)
		return false;

	m_pData = (LPBYTE)LockResource(hGlob);

	// Once we have the file open, we need to read the very first data chunk
	// to see if it's a 3DS file.  That way we don't read an invalid file.
	// If it is a 3DS file, then the first chunk ID will be equal to PRIMARY (some hex num)

	// Read the first chuck of the file to see if it's a 3DS file
	ReadChunk(m_pCurrentChunk);

	// Make sure this is a 3DS file
	if (m_pCurrentChunk->ID != PRIMARY)
	{
		return false;
	}

	// Now we actually start reading in the data.  ProcessNextChunk() is recursive

	// Begin loading objects, by calling this recursive function
	ProcessNextChunk(pModel, m_pCurrentChunk);

	// After we have read the whole 3DS file, we want to calculate our own vertex normals.
	ComputeNormals(pModel);

	// Clean up after everything
	CleanUp();

	return true;
}

///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
///// This function cleans up our allocated memory and closes the file
/////
///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::CleanUp()
{
	delete m_pCurrentChunk;            // Free the current chunk
	delete m_pTempChunk;             // Free our temporary chunk
}


///////////////////////////////// PROCESS NEXT CHUNK\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
///// This function reads the main sections of the .3DS file, then dives deeper with recursion
/////
///////////////////////////////// PROCESS NEXT CHUNK\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ProcessNextChunk(C3DModel *pModel, tChunk *pPreviousChunk)
{
	CMaterialInfo newTexture;       // This is used to add to our material list
	unsigned int version = 0;         // This will hold the file version
	//int buffer[50000] = {0};          // This is used to read past unwanted data
	ZeroMemory(buffer, sizeof(buffer));

	m_pCurrentChunk = new tChunk;        // Allocate a new chunk

	// Below we check our chunk ID each time we read a new chunk.  Then, if
	// we want to extract the information from that chunk, we do so.
	// If we don't want a chunk, we just read past it.

	// Continue to read the sub chunks until we have reached the length.
	// After we read ANYTHING we add the bytes read to the chunk and then check
	// check against the length.

	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// Read next Chunk
		ReadChunk(m_pCurrentChunk);

		// Check the chunk ID

		switch (m_pCurrentChunk->ID)
		{

		case VERSION:             // This holds the version of the file

			// This chunk has an unsigned short that holds the file version.
			// Since there might be new additions to the 3DS file format in 4.0,
			// we give a warning to that problem.

			// Read the file version and add the bytes read to our bytesRead variable
			m_pCurrentChunk->bytesRead += Read(&version, m_pCurrentChunk->length - m_pCurrentChunk->bytesRead);

			// If the file version is over 3, give a warning that there could be a problem

			if (version > 0x03)
				_TRACE(_T("This 3DS file is over version 3 so it may load incorrectly"));

			break;

		case OBJECTINFO:            // This holds the version of the mesh

			// This chunk holds the version of the mesh.  It is also the head of the MATERIAL
			// and OBJECT chunks.  From here on we start reading in the material and object info.

			// Read the next chunk
			ReadChunk(m_pTempChunk);

			// Get the version of the mesh
			m_pTempChunk->bytesRead += Read(&version, m_pTempChunk->length - m_pTempChunk->bytesRead);

			// Increase the bytesRead by the bytes read from the last chunk
			m_pCurrentChunk->bytesRead += m_pTempChunk->bytesRead;

			// Go to the next chunk, which is the object has a texture, it should be MATERIAL, then OBJECT.
			ProcessNextChunk(pModel, m_pCurrentChunk);

			break;

		case MATERIAL:              // This holds the material information

			// This chunk is the header for the material info chunks

			// Increase the number of materials
			pModel->numOfMaterials++;

			// Add a empty texture structure to our texture list.
			// If you are unfamiliar with STL's "vector" class, all Add()
			// does is add a new node onto the list.  I used the vector class
			// so I didn't need to write my own link list functions.
			pModel->pMaterials.Add(newTexture);

			// Proceed to the material loading function
			ProcessNextMaterialChunk(pModel, m_pCurrentChunk);

			break;

		case OBJECT:              // This holds the name of the object being read
		{
			C3DObject newObject;          // This is used to add to our object list

			// This chunk is the header for the object info chunks.  It also
			// holds the name of the object.

			// Increase the object count
			pModel->numOfObjects++;

			// Add a new tObject node to our list of objects (like a link list)
			// Get the name of the object and store it, then add the read bytes to our byte counter.
			m_pCurrentChunk->bytesRead += GetString(newObject.strName);

			pModel->pObject.Add(newObject);

			// Now proceed to read in the rest of the object information
			ProcessNextObjectChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), m_pCurrentChunk);
		}

		break;

		case EDITKEYFRAME:

			// Because I wanted to make this a SIMPLE tutorial as possible, I did not include
			// the key frame information.  This chunk is the header for all the animation info.
			// In a later tutorial this will be the subject and explained thoroughly.

			//ProcessNextKeyFrameChunk(pModel, m_pCurrentChunk);

			// Read past this chunk and add the bytes read to the byte counter
			m_pCurrentChunk->bytesRead += Read(buffer, m_pCurrentChunk->length - m_pCurrentChunk->bytesRead);
			break;

		default:

			// If we didn't care about a chunk, then we get here.  We still need
			// to read past the unknown or ignored chunk and add the bytes read to the byte counter.
			m_pCurrentChunk->bytesRead += Read(buffer, m_pCurrentChunk->length - m_pCurrentChunk->bytesRead);
			break;
		}

		// Add the bytes read from the last chunk to the previous chunk passed in.
		pPreviousChunk->bytesRead += m_pCurrentChunk->bytesRead;
	}

	// Free the current chunk and set it back to the previous chunk (since it started that way)
	delete m_pCurrentChunk;

	m_pCurrentChunk = pPreviousChunk;
}


///////////////////////////////// PROCESS NEXT OBJECT CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
///// This function handles all the information about the objects in the file
/////
///////////////////////////////// PROCESS NEXT OBJECT CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ProcessNextObjectChunk(C3DModel *pModel, C3DObject *pObject, tChunk *pPreviousChunk)
{
	//int buffer[50000] = {0};          // This is used to read past unwanted data
	ZeroMemory(buffer, sizeof(buffer));

	// Allocate a new chunk to work with
	m_pCurrentChunk = new tChunk;

	// Continue to read these chunks until we read the end of this sub chunk

	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// Read the next chunk
		ReadChunk(m_pCurrentChunk);

		// Check which chunk we just read

		switch (m_pCurrentChunk->ID)
		{

		case OBJECT_MESH:         // This lets us know that we are reading a new object

			// We found a new object, so let's read in it's info using recursion
			ProcessNextObjectChunk(pModel, pObject, m_pCurrentChunk);
			break;

		case OBJECT_VERTICES:       // This is the objects vertices
			ReadVertices(pObject, m_pCurrentChunk);
			break;

			//case OBJECT_VERTEXOPTIONS:
			//  ReadVertexOptions(pObject, m_pCurrentChunk);
			//  break;

		case OBJECT_FACES:          // This is the objects face information
			ReadVertexIndices(pObject, m_pCurrentChunk);
			break;

		case OBJECT_MATERIAL:       // This holds the material name that the object has

			// This chunk holds the name of the material that the object has assigned to it.
			// This could either be just a color or a texture map.  This chunk also holds
			// the faces that the texture is assigned to (In the case that there is multiple
			// textures assigned to one object, or it just has a texture on a part of the object.
			// Since most of my game objects just have the texture around the whole object, and
			// they aren't multitextured, I just want the material name.

			// We now will read the name of the material assigned to this object
			ReadObjectMaterial(pModel, pObject, m_pCurrentChunk);
			break;

		case OBJECT_UV:           // This holds the UV texture coordinates for the object

			// This chunk holds all of the UV coordinates for our object.  Let's read them in.
			ReadUVCoordinates(pObject, m_pCurrentChunk);
			break;

		default:

			// Read past the ignored or unknown chunks
			m_pCurrentChunk->bytesRead += Read(buffer, m_pCurrentChunk->length - m_pCurrentChunk->bytesRead);
			break;
		}

		// Add the bytes read from the last chunk to the previous chunk passed in.
		pPreviousChunk->bytesRead += m_pCurrentChunk->bytesRead;
	}

	// Free the current chunk and set it back to the previous chunk (since it started that way)
	delete m_pCurrentChunk;

	m_pCurrentChunk = pPreviousChunk;
}


///////////////////////////////// PROCESS NEXT MATERIAL CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
///// This function handles all the information about the material (Texture)
/////
///////////////////////////////// PROCESS NEXT MATERIAL CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ProcessNextMaterialChunk(C3DModel *pModel, tChunk *pPreviousChunk)
{
	char szBuffer[255] = {0};
	ZeroMemory(buffer, sizeof(buffer));

	// Allocate a new chunk to work with
	m_pCurrentChunk = new tChunk;

	// Continue to read these chunks until we read the end of this sub chunk

	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// Read the next chunk
		ReadChunk(m_pCurrentChunk);

		// Check which chunk we just read in

		switch (m_pCurrentChunk->ID)
		{

		case MATNAME:             // This chunk holds the name of the material
			// Here we read in the material name
			m_pCurrentChunk->bytesRead += Read(pModel->pMaterials[pModel->numOfMaterials - 1].strName, m_pCurrentChunk->length - m_pCurrentChunk->bytesRead);
			break;

		case MATDIFFUSE:            // This holds the R G B color of our object
			ReadColorChunk(&(pModel->pMaterials[pModel->numOfMaterials - 1]), m_pCurrentChunk);
			break;

		case MATMAP:              // This is the header for the texture info

			// Proceed to read in the material information
			ProcessNextMaterialChunk(pModel, m_pCurrentChunk);
			break;

		case MATMAPFILE:            // This stores the file name of the material

			// Here we read in the material's file name
			m_pCurrentChunk->bytesRead += Read(pModel->pMaterials[pModel->numOfMaterials - 1].strFile, m_pCurrentChunk->length - m_pCurrentChunk->bytesRead);
			break;

		default:

			// Read past the ignored or unknown chunks
			m_pCurrentChunk->bytesRead += Read(buffer, m_pCurrentChunk->length - m_pCurrentChunk->bytesRead);
			break;
		}

		// Add the bytes read from the last chunk to the previous chunk passed in.
		pPreviousChunk->bytesRead += m_pCurrentChunk->bytesRead;
	}

	// Free the current chunk and set it back to the previous chunk (since it started that way)
	delete m_pCurrentChunk;

	m_pCurrentChunk = pPreviousChunk;
}

///////////////////////////////// READ CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
///// This function reads in a chunk ID and it's length in bytes
/////
///////////////////////////////// READ CHUNK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadChunk(tChunk *pChunk)
{
	// This reads the chunk ID which is 2 bytes.
	// The chunk ID is like OBJECT or MATERIAL.  It tells what data is
	// able to be read in within the chunks section.
	pChunk->bytesRead = Read(&pChunk->ID, 2);

	// Then, we read the length of the chunk which is 4 bytes.
	// This is how we know how much to read in, or read past.
	pChunk->bytesRead += Read(&pChunk->length, 4);
}

///////////////////////////////// GET STRING \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
///// This function reads in a string of characters
/////
///////////////////////////////// GET STRING \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int CLoad3DS::GetString(char *pBuffer)
{
	int index = 0;

	// Read 1 byte of data which is the first letter of the string
	Read(pBuffer, 1);

	// Loop until we get NULL

	while (*(pBuffer + index++) != 0)
	{
		// Read in a character at a time until we hit NULL.
		Read(pBuffer + index, 1);
	}

	// Return the string length, which is how many bytes we read in (including the NULL)
	return strlen(pBuffer) + 1;
}


///////////////////////////////// READ COLOR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
///// This function reads in the RGB color data
/////
///////////////////////////////// READ COLOR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadColorChunk(CMaterialInfo *pMaterial, tChunk *pChunk)
{
	// Read the color chunk info
	ReadChunk(m_pTempChunk);

	// Read in the R G B color (3 bytes - 0 through 255)
	m_pTempChunk->bytesRead += Read(pMaterial->color, m_pTempChunk->length - m_pTempChunk->bytesRead);

	// Add the bytes read to our chunk
	pChunk->bytesRead += m_pTempChunk->bytesRead;
}


///////////////////////////////// READ VERTEX INDECES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
///// This function reads in the indices for the vertex array
/////
///////////////////////////////// READ VERTEX INDECES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadVertexIndices(C3DObject *pObject, tChunk *pPreviousChunk)
{
	unsigned short index = 0;         // This is used to read in the current face index

	// In order to read in the vertex indices for the object, we need to first
	// read in the number of them, then read them in.  Remember,
	// we only want 3 of the 4 values read in for each face.  The fourth is
	// a visibility flag for 3D Studio Max that doesn't mean anything to us.

	// Read in the number of faces that are in this object (int)
	pPreviousChunk->bytesRead += Read(&pObject->numOfFaces, 2);

	// Alloc enough memory for the faces and initialize the structure
	pObject->pFaces = new CFace[pObject->numOfFaces];
	//memset(pObject->pFaces, 0, sizeof(CFace) * pObject->numOfFaces);

	// Go through all of the faces in this object

	for (int i = 0; i < pObject->numOfFaces; i++)
	{
		// Next, we read in the A then B then C index for the face, but ignore the 4th value.
		// The fourth value is a visibility flag for 3D Studio Max, we don't care about this.
		for (int j = 0; j < 4; j++)
		{
			// Read the first vertice index for the current face
			pPreviousChunk->bytesRead += Read(&index, sizeof(index));

			if (j < 3)
			{
				// Store the index in our face structure.
				pObject->pFaces[i].vertIndex[j] = index;
			}
		}
	}
}


///////////////////////////////// READ UV COORDINATES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
///// This function reads in the UV coordinates for the object
/////
///////////////////////////////// READ UV COORDINATES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadUVCoordinates(C3DObject *pObject, tChunk *pPreviousChunk)
{
	// In order to read in the UV indices for the object, we need to first
	// read in the amount there are, then read them in.

	// Read in the number of UV coordinates there are (int)
	pPreviousChunk->bytesRead += Read(&pObject->numTexVertex, 2);

	// Allocate memory to hold the UV coordinates
	pObject->pTexVerts = new CVector2[pObject->numTexVertex];

	// Read in the texture coodinates (an array 2 float)
	pPreviousChunk->bytesRead += Read(pObject->pTexVerts, pPreviousChunk->length - pPreviousChunk->bytesRead);
}


///////////////////////////////// READ VERTICES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
///// This function reads in the vertices for the object
/////
///////////////////////////////// READ VERTICES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadVertices(C3DObject *pObject, tChunk *pPreviousChunk)
{
	// Like most chunks, before we read in the actual vertices, we need
	// to find out how many there are to read in.  Once we have that number
	// we then Read() them into our vertice array.

	// Read in the number of vertices (int)
	pPreviousChunk->bytesRead += Read(&(pObject->numOfVerts), 2);

	// Allocate the memory for the verts and initialize the structure
	pObject->pVerts = new CVector3[pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(CVector3) * pObject->numOfVerts);

	// Read in the array of vertices (an array of 3 floats)
	pPreviousChunk->bytesRead += Read(pObject->pVerts, pPreviousChunk->length - pPreviousChunk->bytesRead);
}


void CLoad3DS::ReadVertexOptions(C3DObject *pObject, tChunk *pPreviousChunk)
{

}


///////////////////////////////// READ OBJECT MATERIAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
///// This function reads in the material name assigned to the object and sets the materialID
/////
///////////////////////////////// READ OBJECT MATERIAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ReadObjectMaterial(C3DModel *pModel, C3DObject *pObject, tChunk *pPreviousChunk)
{
	char strMaterial[255] = {0};      // This is used to hold the objects material name
	ZeroMemory(buffer, sizeof(buffer));

	// *What is a material?*  - A material is either the color or the texture map of the object.
	// It can also hold other information like the brightness, shine, etc... Stuff we don't
	// really care about.  We just want the color, or the texture map file name really.

	// Here we read the material name that is assigned to the current object.
	// strMaterial should now have a string of the material name, like "Material #2" etc..
	pPreviousChunk->bytesRead += GetString(strMaterial);

	// Now that we have a material name, we need to go through all of the materials
	// and check the name against each material.  When we find a material in our material
	// list that matches this name we just read in, then we assign the materialID
	// of the object to that material index.  You will notice that we passed in the
	// model to this function.  This is because we need the number of textures.
	// Yes though, we could have just passed in the model and not the object too.

	// Go through all of the textures

	for (int i = 0; i < pModel->numOfMaterials; i++)
	{
		// If the material we just read in matches the current texture name
		if (strcmp(pModel->pMaterials[i].strName, strMaterial) == 0)
		{
			// Set the material ID to the current index 'i' and stop checking
			pObject->materialID = i;

			// Now that we found the material, check if it's a texture map.
			// If the strFile has a string length of 1 and over it's a texture

			if (strlen(pModel->pMaterials[i].strFile) > 0)
			{
				// Set the object's flag to say it has a texture map to bind.
				pObject->bHasTexture = true;
			}

			break;
		}
		else
		{
			// Set the ID to -1 to show there is no material for this object
			pObject->materialID = -1;
		}
	}

	// Read past the rest of the chunk since we don't care about shared vertices
	// You will notice we subtract the bytes already read in this chunk from the total length.
	pPreviousChunk->bytesRead += Read(buffer, pPreviousChunk->length - pPreviousChunk->bytesRead);
}


///////////////////////////////// COMPUTER NORMALS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
///// This function computes the normals and vertex normals of the objects
/////
///////////////////////////////// COMPUTER NORMALS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoad3DS::ComputeNormals(C3DModel *pModel)
{
	CVector3 vVector1, vVector2, vNormal, vPoly[3];

	// If there are no objects, we can skip this part

	if (pModel->numOfObjects <= 0)
		return;

	// What are vertex normals?  And how are they different from other normals?
	// Well, if you find the normal to a triangle, you are finding a "Face Normal".
	// If you give OpenGL a face normal for lighting, it will make your object look
	// really flat and not very round.  If we find the normal for each vertex, it makes
	// the smooth lighting look.  This also covers up blocky looking objects and they appear
	// to have more polygons than they do.    Basically, what you do is first
	// calculate the face normals, then you take the average of all the normals around each
	// vertex.  It's just averaging.  That way you get a better approximation for that vertex.

	// Go through each of the objects to calculate their normals
	for (int index = 0; index < pModel->numOfObjects; index++)
	{
		// Get the current object
		C3DObject *pObject = &(pModel->pObject[index]);

		// Here we allocate all the memory we need to calculate the normals
		CVector3 *pNormals    = new CVector3 [pObject->numOfFaces];
		CVector3 *pTempNormals  = new CVector3 [pObject->numOfFaces];
		pObject->pNormals   = new CVector3 [pObject->numOfVerts];

		// Go though all of the faces of this object

		for (int i = 0; i < pObject->numOfFaces; i++)
		{
			// To cut down LARGE code, we extract the 3 points of this face
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

			// Now let's calculate the face normals (Get 2 vectors and find the cross product of those 2)
			CVector3 vVector1(vPoly[0], vPoly[1]);    // Get the vector of the polygon (we just need 2 sides for the normal)
			CVector3 vVector2(vPoly[1], vPoly[2]);    // Get a second vector of the polygon

			vNormal  = Cross(vVector1, vVector2);   // Return the cross product of the 2 vectors (normalize vector, but not a unit vector)
			pTempNormals[i] = vNormal;          // Save the un-normalized normal for the vertex normals
			vNormal.Normalize();        // Normalize the cross product to give us the polygons normal

			pObject->pFaces[i].vFaceNormal = vNormal;
			pNormals[i] = vNormal;            // Assign the normal to the list of normals
		}

		//////////////// Now Get The Vertex Normals /////////////////
		CVector3 vSum;

		CVector3 vZero;

		int shared = 0;

		for (int i = 0; i < pObject->numOfVerts; i++)    // Go through all of the vertices
		{
			for (int j = 0; j < pObject->numOfFaces; j++) // Go through all of the triangles
			{
				// Check if the vertex is shared by another face

				if (pObject->pFaces[j].vertIndex[0] == i ||
						pObject->pFaces[j].vertIndex[1] == i ||
						pObject->pFaces[j].vertIndex[2] == i)
				{
					CVector3 n = pTempNormals[j];
					vSum = vSum + n;// Add the un-normalized normal of the shared face
					shared++;               // Increase the number of shared triangles
				}
			}

			// Get the normal by dividing the sum by the shared.  We negate the shared so it has the normals pointing out.
			pObject->pNormals[i] = vSum / float(shared);

			// Normalize the normal for the final vertex normal
			pObject->pNormals[i].Normalize();

			vSum = vZero;                 // Reset the sum

			shared = 0;                   // Reset the shared
		}

		// Free our memory and start over on the next object
		delete [] pTempNormals;

		delete [] pNormals;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
unsigned int CLoad3DS::Read(void* pBuffer, unsigned int nLength)
{
	memcpy(pBuffer, m_pData, nLength);
	m_pData += nLength;
	return nLength;
}
