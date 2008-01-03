#include "stdafx.h"
#include "Geometry.h"

CVector2::CVector2()
{
	x = y = 0.0f;
}

CVector3::CVector3()
{
	x = y = z = 0.0f;
}

// *Note*
//
// Below are some math functions for calculating vertex normals.  We want vertex normals
// because it makes the lighting look really smooth and life like.  You probably already
// have these functions in the rest of your engine, so you can delete these and call
// your own.  I wanted to add them so I could show how to calculate vertex normals.

//////////////////////////////  Math Functions  ////////////////////////////////*


// This divides a vector by a single number (scalar) and returns the result

// This returns the cross product between 2 vectors
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vCross;                // The vector to hold the cross product
	// Get the X value
	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	// Get the Y value
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	// Get the Z value
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vCross;                // Return the cross product
}

