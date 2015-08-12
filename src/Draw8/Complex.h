#include "pch.h"

#ifndef DEF_COMPLEX
#define DEF_COMPLEX

#include <cmath>
#include <xtgmath.h>
#include <complex>

#include "Vector2.h"

namespace Draw8
{
	public ref class Complex sealed
	{
		public:	
			property float Real;
			property float Imaginary;

			property float Argument { float get() { return atan2(Imaginary, Real); } }
			property float SquaredLength { float get() { return Real*Real + Imaginary*Imaginary; } }
			property float Length { float get() { return sqrt(Real*Real + Imaginary*Imaginary); } void set(float value) { Normalize(); Real *= value; Imaginary *= value; } }

			property static Complex^ ImaginaryOne { Complex^ get() { return ref new Complex(0, 1); } }
			property static Complex^ I { Complex^ get() { return ImaginaryOne; }}

		public:
			Complex() { this->Real = 0; this->Imaginary = 0; }
			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			Complex(float Real) { this->Real = Real; this->Imaginary = 0; }
			Complex(Vector2^ Vector) { this->Real = Vector->X; this->Imaginary = Vector->Y; }
			Complex(float Real, float Imaginary) { this->Real = Real; this->Imaginary = Imaginary; }
		internal:
			Complex(std::complex<float> UnderlyingComplex) { this->Real = UnderlyingComplex.real(); this->Imaginary = UnderlyingComplex.imag(); }

		public:
			Vector2^ AsVector() { return ref new Vector2(Real, Imaginary); }

			static Complex^ FromPolar(float Angle, float Length) { return ref new Complex(cos(Angle) * Length, sin(Angle) * Length); }
			static Complex^ FromCartesian(float Real, float Imaginary) { return ref new Complex(Real, Imaginary); }

			static Complex^ Invert(Complex^);
			void Invert();
			
			static Complex^ Normalize(Complex^ Complex);
			void Normalize();

			static Complex^ Conjugate(Complex^ Complex) { return ref new Draw8::Complex(Complex->Real, -Complex->Imaginary); }
			void Conjugate() { Imaginary = -Imaginary; }

			///<summary>Returns the object as a string.</summary>
			Platform::String^ ToString();

		private:
			static bool operator==(Complex^ a, Complex^ b) { return a->Real == b->Real && a->Imaginary == b->Imaginary; }
			static bool operator!=(Complex^ a, Complex^ b) { return a->Real != b->Real || a->Imaginary != b->Imaginary; }
			//static bool operator==(Complex^ a, float b) { return a->Real == b && a->Imaginary == 0; }
			//static bool operator!=(Complex^ a, float b) { return a->Real != b || a->Imaginary != 0; }
			//static bool operator==(float b, Complex^ a) { return a->Real == b && a->Imaginary == 0; }
			//static bool operator!=(float b, Complex^ a) { return a->Real != b || a->Imaginary != 0; }


			static operator Complex^ (int Value) { return ref new Complex((float)Value); }
			static operator Complex ^ (short Value) { return ref new Complex((float)Value); }
			static operator Complex ^ (float Value) { return ref new Complex((float)Value); }
			static operator Complex ^ (double Value) { return ref new Complex((float)Value); }

			static Complex^ operator+(Complex^ a, Complex^ b) { return ref new Complex(a->Real + b->Real, a->Imaginary + b->Imaginary); }
			static Complex^ operator-(Complex^ a) { return ref new Complex(-a->Real, -a->Imaginary); }
			static Complex^ operator-(Complex^ a, Complex^ b) { return ref new Complex(a->Real - b->Real, a->Imaginary - b->Imaginary); }
			static Complex^	operator*(Complex^, Complex^);
			static Complex^	operator/(Complex^, Complex^);

			//static Complex^ operator+(Complex^ a, float b) { return ref new Complex(a->Real + b, a->Imaginary); }
			//static Complex^ operator-(Complex^ a, float b) { return ref new Complex(a->Real - b, a->Imaginary); }
			//static Complex^	operator*(Complex^, float);
			//static Complex^	operator/(Complex^, float);

			//static Complex^ operator+(float b, Complex^ a) { return ref new Complex(a->Real + b, a->Imaginary); }
			//static Complex^ operator-(float b, Complex^ a) { return ref new Complex(a->Real - b, a->Imaginary); }
			//static Complex^	operator*(float, Complex^);
			//static Complex^	operator/(float, Complex^);

		internal:
			std::complex<float> AsComplex() { return std::complex<float>(Real, Imaginary); }
	};
}

#endif
