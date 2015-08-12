#include "pch.h"
#include "Complex.h"

Draw8::Complex^ Draw8::Complex::operator*(Draw8::Complex^ x, Draw8::Complex^ y)
{
	float a = x->Real;
	float b = x->Imaginary;
	float c = y->Real;
	float d = y->Imaginary;
	
	return ref new Draw8::Complex(a * c - b * d, a * d + b * c);
}

Draw8::Complex^ Draw8::Complex::operator/(Draw8::Complex^ x, Draw8::Complex^ y)
{
	float a = x->Real;
	float b = x->Imaginary;
	float c = y->Real;
	float d = y->Imaginary;
	float norm = 1.f / (c*c + d*d);

	return ref new Draw8::Complex((a*c + b*d) * norm, (b*c - a*d) * norm);
}
//
//Draw8::Complex^ Draw8::Complex::operator*(Draw8::Complex^ x, float y)
//{
//	float a = x->Real;
//	float b = x->Imaginary;
//
//	return ref new Draw8::Complex(a * y, b * y);
//}
//
//Draw8::Complex^ Draw8::Complex::operator/(Draw8::Complex^ x, float y)
//{
//	float a = x->Real;
//	float b = x->Imaginary;
//	float oY = 1.f / y;
//
//	return ref new Draw8::Complex(a * oY, b / oY);
//}
//
//Draw8::Complex^ Draw8::Complex::operator*(float y, Draw8::Complex^ x)
//{
//	float a = x->Real;
//	float b = x->Imaginary;
//
//	return ref new Draw8::Complex(a * y, b * y);
//}
//
//Draw8::Complex^ Draw8::Complex::operator/(float y, Draw8::Complex^ x)
//{
//	float a = x->Real;
//	float b = x->Imaginary;
//	float oY = 1.f / y;
//
//	return ref new Draw8::Complex(a * oY, b / oY);
//}

Draw8::Complex^ Draw8::Complex::Invert(Draw8::Complex^ x) 
{ 
	float a = x->Real;
	float b = x->Imaginary;
	float norm = 1.f / (a*a + b*b);

	return ref new Draw8::Complex(a * norm, -b * norm);
}
void Draw8::Complex::Invert()
{
	float norm = 1.f / SquaredLength;

	Real *= norm;
	Imaginary *= -norm;
}

Draw8::Complex^ Draw8::Complex::Normalize(Draw8::Complex^ Complex)
{
	float norm = 1.f / Complex->Length;

	return ref new Draw8::Complex(Complex->Real * norm, Complex->Imaginary * norm);
}

void Draw8::Complex::Normalize()
{
	float norm = 1.f / Length;

	Real *= norm;
	Imaginary *= norm;
}

Platform::String^ Draw8::Complex::ToString()
{
	/*if (Real == 0 && Imaginary)
		return "0";
	else(Real == )
	*/
	return (Real != 0 ? Real.ToString() : "") + (Imaginary != 0 && Real != 0 ? " + " : "") + (Imaginary != 0 ? "i" + Imaginary.ToString() : "");
}