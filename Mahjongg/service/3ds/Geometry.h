#pragma once

// This is our 2D point class.  This will be used to store the UV coordinates.

class CVector2
{

public:
	float x, y;

	CVector2();
};

// This is our 3D point class.  This will be used to store the vertices of our model.

class CVector3
{

public:
	float x, y, z;

	CVector3();
	CVector3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	CVector3(const CVector3& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
	}

	CVector3(CVector3& vector1, CVector3& vector2)
	{
		x = vector1.x - vector2.x;      // Subtract point1 and point2 x's
		y = vector1.y - vector2.y;      // Subtract point1 and point2 y's
		z = vector1.z - vector2.z;      // Subtract point1 and point2 z's
	}

	CVector3& Normalize()
	{
		double Magnitude = sqrt(x * x + y * y + z * z);   // Get the magnitude

		x /= (float)Magnitude;        // Divide the vector's X by the magnitude
		y /= (float)Magnitude;        // Divide the vector's Y by the magnitude
		z /= (float)Magnitude;        // Divide the vector's Z by the magnitude

		return *this;               // Return the normal
	}

	friend CVector3 operator +(const CVector3& vector1, const CVector3& vector2);
	friend CVector3 operator *(CVector3& vector1, float value);
	friend CVector3 operator /(CVector3& vector1, float value);
};
inline CVector3 Cross(CVector3& vVector1, CVector3& vVector2)
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

///////////////////////////////////////////////////////////////////////////////////////////////////
// This adds 2 vectors together and returns the result
inline CVector3 operator+(CVector3& vVector1, CVector3& vVector2)
{
	// Return the resultant vector
	return CVector3(vVector2.x + vVector1.x, vVector2.y + vVector1.y, vVector2.z + vVector1.z);
}


inline CVector3 operator/(CVector3& vVector1, float value)
{
	// Return the resultant vector
	return CVector3(vVector1.x / value, vVector1.y / value, vVector1.z / value);
}


inline CVector3 operator*(CVector3& vVector1, float value)
{
	// Return the resultant vector
	return CVector3(vVector1.x * value, vVector1.y * value, vVector1.z * value);
}
