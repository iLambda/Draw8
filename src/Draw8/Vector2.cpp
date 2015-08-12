#include "pch.h"
#include "Vector2.h"
#include "Complex.h"

Draw8::Vector2::Vector2()
{
	this->X = 0;
	this->Y = 0;
}

Draw8::Vector2::Vector2(float Scale)
{
	this->X = Scale;
	this->Y = Scale;
}

Draw8::Vector2::Vector2(float X, float Y)
{
	this->X = X;
	this->Y = Y;
}

Draw8::Vector2::Vector2(Draw8::Vector2^ Copy)
{
	this->X = Copy->X;
	this->Y = Copy->Y;
}


Draw8::Vector2^ Draw8::Vector2::Normalize(Draw8::Vector2^ Vector)
{
	auto length = Vector->Length;
	return ref new Draw8::Vector2(Vector->X / length, Vector->Y / length);
}

void Draw8::Vector2::Normalize()
{
	auto length = this->Length;
	this->X /= length;
	this->Y /= length;
}

float Draw8::Vector2::Dot(Draw8::Vector2^ VectorA, Draw8::Vector2^ VectorB)
{
	return VectorA->X * VectorB->X + VectorA->Y * VectorB->Y;
}

float Draw8::Vector2::Dot(Draw8::Vector2^ Vector)
{
	return this->X * Vector->X + this->Y * Vector->Y;
}

bool Draw8::Vector2::operator==(Draw8::Vector2^ a, Draw8::Vector2^ b)
{ 
	return (a->X == b->X) && (a->Y == b->Y);
}

bool Draw8::Vector2::operator!=(Draw8::Vector2^ a, Draw8::Vector2^ b)
{
	return (a->X != b->X) || (a->Y != b->Y);
}

Draw8::Vector2^ Draw8::Vector2::operator+(Draw8::Vector2^ a, Draw8::Vector2^ b)
{
	return ref new Vector2(a->X + b->X, a->Y + b->Y);
}

Draw8::Vector2^ Draw8::Vector2::operator-(Draw8::Vector2^ a)
{
	return ref new Vector2(-a->X, -a->Y);
}

Draw8::Vector2^ Draw8::Vector2::operator-(Draw8::Vector2^ a, Draw8::Vector2^ b)
{
	return ref new Vector2(a->X - b->X, a->Y - b->Y);
}

Draw8::Vector2^ Draw8::Vector2::operator*(Draw8::Vector2^ a, Draw8::Vector2^ b)
{
	return ref new Vector2(a->X * b->X, a->Y * b->Y);
}

Draw8::Vector2^ Draw8::Vector2::operator/(Draw8::Vector2^ a, Draw8::Vector2^ b)
{
	return ref new Vector2(a->X / b->X, a->Y / b->Y);
}

Draw8::Vector2^ Draw8::Vector2::operator*(Draw8::Vector2^ a, float b)
{
	return ref new Vector2(a->X * b, a->Y * b);
}

Draw8::Vector2^ Draw8::Vector2::operator/(Draw8::Vector2^ a, float b) 
{
	return ref new Vector2(a->X / b, a->Y / b);
}

Draw8::Complex^ Draw8::Vector2::GetAffix()
{
	return ref new Complex(X, Y);
}