#include "pch.h"

#ifndef DEF_VECTOR2
#define DEF_VECTOR2

#include <math.h>

namespace Draw8
{
	ref class Complex;

	///<summary>A 2D-geometric quantity containing a magnitude, and direction. Used to represent positions or directions.</summary>
	public ref class Vector2 sealed : public Platform::Object
	{
		public:
			///<summary>The X coordinate of the vector.</summary>
			property float X;
			///<summary>The Y coordinate of the vector.</summary>
			property float Y;

			///<summary>The angle between the vector and (1, 0) in radians.</summary>	
			property float Angle { float get() { return atan2(Y, X); } }
			///<summary>The squared length(magnitude) of the vector. No square root makes it more fitted for real time. Use for comparisons.</summary>
			property float SquaredLength { float get() { return X*X + Y*Y; } }
			///<summary>The length(magnitude) of the vector. Square root makes it less fitted for real time.</summary>
			property float Length 
			{ 
				float get() { return sqrt(X*X + Y*Y); } 
				void set(float value) { Normalize(); X *= value; Y *= value; } 
			}
			
		public:
			///<summary>The null vector.</summary>
			property static Vector2^ Null { Vector2^ get() { return ref new Vector2(0); } }

		public:
			///<summary>Creates a vector representing the origin of the world.</summary>
			Vector2();
			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Creates a vector with all coordinates equal to scale.</summary>
			///<param name="Scale">The X and Y coordinate.</param>
			Vector2(float Scale);
			///<summary>Copies a vector.</summary>
			///<param name="Copy">The vector to copy.</param>
			Vector2(Vector2^ Copy);
			///<summary>Creates a vector having the two coordinates passed in parameter.</summary>
			///<param name="X">The X coordinate of the vector.</param>
			///<param name="Y">The Y coordinate of the vector.</param>
			Vector2(float X, float Y);
			

		public:
			///<summary>Set the vector's length to one while keeping his direction.</summary>
			///<param name="Vector">The vector to operate on.</param>
			///<returns>The normalized vector.</returns>
			static Vector2^ Normalize(Vector2^ Vector); 
			///<summary>Computes dot product of two vectors.</summary>
			///<param name="VectorA">One of the vectors to operate on.</param>
			///<param name="VectorB">The other vector to operate on.</param>
			///<returns>The dot product.</returns>
			static float Dot(Vector2^ VectorA, Vector2^ VectorB); 

			///<summary>Set the vector's length to one while keeping his direction.</summary>
			void Normalize();	
			///<summary>Computes dot product of this vector and another.</summary>
			///<param name="Vector">The other vector to operate on.</param>
			///<returns>The dot product.</returns>
			float Dot(Vector2^ Vector);
			///<summary>Returns the object as a string.</summary>
			Platform::String^ ToString() { return "(" + X + ";" + Y + ")"; }
			///<summary>Returns the affix of the vector.</summary>
			Complex^ GetAffix();

		private:
			static bool operator==(Vector2^, Vector2^);
			static bool operator!=(Vector2^, Vector2^);
			static Vector2^ operator+(Vector2^, Vector2^);
			static Vector2^ operator-(Vector2^);
			static Vector2^ operator-(Vector2^, Vector2^ );
			static Vector2^	operator*(Vector2^, Vector2^ );
			static Vector2^	operator/(Vector2^, Vector2^ );
			static Vector2^ operator*(Vector2^, float );
			static Vector2^ operator/(Vector2^, float );
	};
}

#endif
