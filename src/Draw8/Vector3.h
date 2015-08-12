#include "pch.h"

#ifndef DEF_VECTOR3
#define DEF_VECTOR3

#include <math.h>
#include "Vector2.h"

namespace Draw8
{
	///<summary>A 3D-geometric quantity containing a magnitude, and direction. Used to represent positions or directions.</summary>
	public ref class Vector3 sealed : public Platform::Object
	{
		public:
			///<summary>The X coordinate of the vector.</summary>
			property float X;
			///<summary>The Y coordinate of the vector.</summary>
			property float Y;
			///<summary>The Z coordinate of the vector.</summary>
			property float Z;

			///<summary>The squared length(magnitude) of the vector. No square root makes it more fitted for real time. Use for comparisons.</summary>
			property float SquaredLength { float get() { return X*X + Y*Y + Z*Z; } }
			///<summary>The length(magnitude) of the vector. Square root makes it less fitted for real time.</summary>
			property float Length 
			{ 
				float get() { return sqrt(X*X + Y*Y + Z*Z); } 
				void set(float value) { Normalize(); X *= value; Y *= value; Z *= value; } 
			}
			
		public:
			///<summary>The null vector.</summary>
			property static Vector3^ Null { Vector3^ get() { return ref new Vector3(0); } }

		public:
			///<summary>Creates a vector representing the origin of the world.</summary>
			Vector3();
			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Creates a vector with all coordinates equal to scale.</summary>
			///<param name="Scale">The X, Y and Z coordinate.</param>
			Vector3(float Scale);
			///<summary>Copies a vector.</summary>
			///<param name="Copy">The vector to copy.</param>
			Vector3(Vector3^ Copy);
			///<summary>Creates a vector having the three coordinates passed in parameter.</summary>
			///<param name="Vector">The X and Y coords stored in a 2D vector.</param>
			///<param name="Z">The Z coordinate of the vector.</param>
			Vector3(Vector2^ Vector, float Z);
			///<summary>Creates a vector having the three coordinates passed in parameter.</summary>
			///<param name="X">The X coordinate of the vector.</param>
			///<param name="Y">The Y coordinate of the vector.</param>
			///<param name="Z">The Z coordinate of the vector.</param>
			Vector3(float X, float Y, float Z);
			

		public:
			///<summary>Set the vector's length to one while keeping his direction.</summary>
			///<param name="Vector">The vector to operate on.</param>
			///<returns>The normalized vector.</returns>
			static Vector3^ Normalize(Vector3^ Vector); 
			///<summary>Computes dot product of two vectors.</summary>
			///<param name="VectorA">One of the vectors to operate on.</param>
			///<param name="VectorB">The other vector to operate on.</param>
			///<returns>The dot product.</returns>
			static float Dot(Vector3^ VectorA, Vector3^ VectorB); 

			///<summary>Set the vector's length to one while keeping his direction.</summary>
			void Normalize();
			///<summary>Computes dot product of this vector and another.</summary>
			///<param name="Vector">The other vector to operate on.</param>
			///<returns>The dot product.</returns>
			float Dot(Vector3^ Vector);
			///<summary>Returns the object as a string.</summary>
			Platform::String^ ToString() { return "(" + X + ";" + Y + ";" + Z + ")"; }

		private:
			static bool operator==(Vector3^, Vector3^);
			static bool operator!=(Vector3^, Vector3^);
			static Vector3^ operator+(Vector3^, Vector3^ );
			static Vector3^ operator-(Vector3^, Vector3^ );
			static Vector3^ operator-(Vector3^);
			static Vector3^	operator*(Vector3^, Vector3^ );
			static Vector3^	operator/(Vector3^, Vector3^ );
			static Vector3^ operator*(Vector3^, float );
			static Vector3^ operator/(Vector3^, float );

	};
}

#endif
