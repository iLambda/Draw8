#include "pch.h"
#include "Vector3.h"

Draw8::Vector3::Vector3()
{
	this->X = 0;
	this->Y = 0;
	this->Z = 0;
}

Draw8::Vector3::Vector3(float Scale)
{
	this->X = Scale;
	this->Y = Scale;
	this->Z = Scale;
}

Draw8::Vector3::Vector3(Vector2^ Vector, float Z)
{
	this->X = Vector->X;
	this->Y = Vector->Y;
	this->Z = Z;
}

Draw8::Vector3::Vector3(float X, float Y, float Z)
{
	this->X = X;
	this->Y = Y;
	this->Z = Z;
}

Draw8::Vector3::Vector3(Draw8::Vector3^ Copy)
{
	this->X = Copy->X;
	this->Y = Copy->Y;
	this->Z = Copy->Z;
}


Draw8::Vector3^ Draw8::Vector3::Normalize(Draw8::Vector3^ Vector)
{
	auto length = Vector->Length;
	return ref new Draw8::Vector3(Vector->X / length, Vector->Y / length, Vector->Z / length);
}

void Draw8::Vector3::Normalize()
{
	auto length = this->Length;
	this->X /= length;
	this->Y /= length;
	this->Z /= length;
}

float Draw8::Vector3::Dot(Draw8::Vector3^ VectorA, Draw8::Vector3^ VectorB)
{
	return VectorA->X * VectorB->X + VectorA->Y * VectorB->Y + VectorA->Z * VectorB->Z;
}

float Draw8::Vector3::Dot(Draw8::Vector3^ Vector)
{
	return this->X * Vector->X + this->Y * Vector->Y + this->Z * Vector->Z;
}

bool Draw8::Vector3::operator==(Draw8::Vector3^ a, Draw8::Vector3^ b)
{ 
	return (a->X == b->X) && (a->Y == b->Y) && (a->Z == b->Z);
}

bool Draw8::Vector3::operator!=(Draw8::Vector3^ a, Draw8::Vector3^ b)
{
	return (a->X != b->X) || (a->Y != b->Y) || (a->Z != b->Z);
}

Draw8::Vector3^ Draw8::Vector3::operator+(Draw8::Vector3^ a, Draw8::Vector3^ b)
{
	return ref new Vector3(a->X + b->X, a->Y + b->Y, a->Z + b->Z);
}

Draw8::Vector3^ Draw8::Vector3::operator-(Draw8::Vector3^ a, Draw8::Vector3^ b)
{
	return ref new Vector3(a->X - b->X, a->Y - b->Y, a->Z - b->Z);
}

Draw8::Vector3^ Draw8::Vector3::operator-(Draw8::Vector3^ a)
{
	return ref new Vector3(-a->X, -a->Y, -a->Z);
}

Draw8::Vector3^ Draw8::Vector3::operator*(Draw8::Vector3^ a, Draw8::Vector3^ b)
{
	return ref new Vector3(a->X * b->X, a->Y * b->Y, a->Z * b->Z);
}

Draw8::Vector3^ Draw8::Vector3::operator/(Draw8::Vector3^ a, Draw8::Vector3^ b)
{
	return ref new Vector3(a->X / b->X, a->Y / b->Y, a->Z * b->Z);
}

Draw8::Vector3^ Draw8::Vector3::operator*(Draw8::Vector3^ a, float b)
{
	return ref new Vector3(a->X * b, a->Y * b, a->Z * b);
}

Draw8::Vector3^ Draw8::Vector3::operator/(Draw8::Vector3^ a, float b) 
{
	return ref new Vector3(a->X / b, a->Y / b, a->Z / b);
}