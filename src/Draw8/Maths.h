#include "pch.h"

#ifndef DEF_MATH
#define DEF_MATH

#include <math.h>
#include <xtgmath.h>
#include <cmath>
#include <ymath.h>
#include "Complex.h"

#define M_E   2.7182818284590452354
#define M_PI   3.14159265358979323846
#define M_PI_2   1.57079632679489661923
#define M_PI_4   0.78539816339744830962
#define M_1_PI   0.31830988618379067154
#define M_2_PI   0.63661977236758134308

namespace Draw8
{
	public ref class Maths sealed
	{
		public:
			///<summary>Returns pi, the ratio of a circle's circumference to its diameter.</summary>
			static property double Pi { double get() { return M_PI; } }
			///<summary>Returns pi divided by 2.</summary>
			static property double PiOver2 { double get() { return M_PI_2; } }
			///<summary>Returns pi divided by 4.</summary>
			static property double PiOver4 { double get() { return M_PI_4; } }
			///<summary>Returns one divided by pi.</summary>
			static property double OneOverPi { double get() { return M_1_PI; } }
			///<summary>Returns two divided by pi.</summary>
			static property double TwoOverPi { double get() { return M_2_PI; } }
			///<summary>Returns e.</summary>
			static property double E { double get() { return M_E; } }
			///<summary>Returns the golden ratio, (sqrt(5) + 1) / 2.</summary>
			static property double Phi { double get() { return PHI; } }
	
		private:
			//Private constructor
			Maths();

		public:
			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the cosine of the angle.</summary>
			///<param name="Angle">The angle.</param>
			static float Cos(float Angle) { return cos(Angle); }
			///<summary>Returns the cosine of the angle.</summary>
			///<param name="Angle">The angle.</param>
			static double Cos(double Angle) { return cos(Angle); }
			///<summary>Returns the sine of a complex value.</summary>
			///<param name="Complex">The value.</param>
			static Draw8::Complex^ Cos(Complex^ Complex) { return ref new Draw8::Complex(std::cos(Complex->AsComplex())); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the arc cosine of a value.</summary>
			///<param name="Value">The value.</param>
			static float Acos(float Value) { return acos(Value); }
			///<summary>Returns the arc cosine of a value.</summary>
			///<param name="Value">The value.</param>
			static double Acos(double Value) { return acos(Value); }
			///<summary>Returns the arc cosine of a complex value.</summary>
			///<param name="Complex">The value.</param>
			static Draw8::Complex^ Acos(Complex^ Complex) { return ref new Draw8::Complex(std::acos(Complex->AsComplex())); }
			
			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the sine of the angle.</summary>
			///<param name="Angle">The angle.</param>
			static float Sin(float Angle) { return sin(Angle); }
			///<summary>Returns the sine of the angle.</summary>
			///<param name="Angle">The angle.</param>
			static double Sin(double Angle) { return sin(Angle); }
			///<summary>Returns the sine of a complex value.</summary>
			///<param name="Complex">The value.</param>
			static Draw8::Complex^ Sin(Complex^ Complex) { return ref new Draw8::Complex(std::sin(Complex->AsComplex())); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the arc sine of a value.</summary>
			///<param name="Value">The value.</param>
			static float Asin(float Value) { return asin(Value); }
			///<summary>Returns the arc sine of a value.</summary>
			///<param name="Value">The value.</param>
			static double Asin(double Value) { return asin(Value); }
			///<summary>Returns the arc sine of a complex value.</summary>
			///<param name="Complex">The value.</param>
			static Draw8::Complex^ Asin(Complex^ Complex) { return ref new Draw8::Complex(std::asin(Complex->AsComplex())); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the tangent of the angle.</summary>
			///<param name="Angle">The angle.</param>
			static float Tan(float Angle) { return tan(Angle); }
			///<summary>Returns the tangent of the angle.</summary>
			///<param name="Angle">The angle.</param>
			static double Tan(double Angle) { return tan(Angle); }
			///<summary>Returns the tangent of a complex value.</summary>
			///<param name="Complex">The value.</param>
			static Draw8::Complex^ Tan(Complex^ Complex) { return ref new Draw8::Complex(std::tan(Complex->AsComplex())); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the arc tangent of a value.</summary>
			///<param name="Value">The value.</param>
			static float Atan(float Value) { return atan(Value); }
			///<summary>Returns the arc tangent of a value.</summary>
			///<param name="Value">The value.</param>
			static double Atan(double Value) { return atan(Value); }
			///<summary>Returns the arc tangent of a complex value.</summary>
			///<param name="Complex">The value.</param>
			static Draw8::Complex^ Atan(Complex^ Complex) { return ref new Draw8::Complex(std::atan(Complex->AsComplex())); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the argument of the complex represented by X + iY (the angle between (X, Y) and (1, 0)).</summary>
			///<param name="X">The X value.</param>
			///<param name="Y">The Y value.</param>
			static float Atan2(float X, float Y) { return atan2(X, Y); }
			///<summary>Returns the argument of the complex represented by X + iY (the angle between (X, Y) and (1, 0)).</summary>
			///<param name="X">The X value.</param>
			///<param name="Y">The Y value.</param>
			static double Atan2(double X, double Y) { return atan2(X, Y); }
			///<summary>Returns the argument of the complex (the angle between (Real, imaginary) and (1, 0)).</summary>
			///<param name="Complex">The value.</param>
			static float Atan2(Complex^ Complex) { return Complex->Argument; }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the hyperbolic cosine of a value.</summary>
			///<param name="Value">The value.</param>
			static float Cosh(float Value) { return cosh(Value); }
			///<summary>Returns the hyperbolic cosine of a value.</summary>
			///<param name="Value">The value.</param>
			static double Cosh(double Value) { return cosh(Value); }
			///<summary>Returns the hyperbolic cosine of a complex value.</summary>
			///<param name="Complex">The value.</param>
			static Draw8::Complex^ Cosh(Complex^ Complex) { return ref new Draw8::Complex(std::cosh(Complex->AsComplex())); }
			
			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the arc hyperbolic cosine of a value.</summary>
			///<param name="Value">The value.</param>
			static float Acosh(float Value) { return std::acosh(std::complex<float>(Value, 0)).real(); }
			///<summary>Returns the arc hyperbolic cosine of a value.</summary>
			///<param name="Value">The value.</param>
			static double Acosh(double Value) { return std::acosh(std::complex<double>(Value, 0)).real(); }
			///<summary>Returns the arc hyperbolic cosine of a complex value.</summary>
			///<param name="Complex">The value.</param>
			static Draw8::Complex^ Acosh(Complex^ Complex) { return ref new Draw8::Complex(std::acosh(Complex->AsComplex())); }
			
			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the hyperbolic sine of a value.</summary>
			///<param name="Value">The value.</param>
			static float Sinh(float Value) { return sinh(Value); }
			///<summary>Returns the hyperbolic sine of a value.</summary>
			///<param name="Value">The value.</param>
			static double Sinh(double Value) { return sinh(Value); }
			///<summary>Returns the hyperbolic sine of a complex value.</summary>
			///<param name="Complex">The value.</param>
			static Draw8::Complex^ Sinh(Complex^ Complex) { return ref new Draw8::Complex(std::sinh(Complex->AsComplex())); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the arc hyperbolic sine of a value.</summary>
			///<param name="Value">The value.</param>
			static float Asinh(float Value) { return std::asinh(std::complex<float>(Value, 0)).real(); }
			///<summary>Returns the arc hyperbolic sine of a value.</summary>
			///<param name="Value">The value.</param>
			static double Asinh(double Value) { return std::asinh(std::complex<double>(Value, 0)).real(); }
			///<summary>Returns the arc hyperbolic sine of a complex value.</summary>
			///<param name="Complex">The value.</param>
			static Draw8::Complex^ Asinh(Complex^ Complex) { return ref new Draw8::Complex(std::asinh(Complex->AsComplex())); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the hyperbolic tangent of a value.</summary>
			///<param name="Value">The value.</param>
			static float Tanh(float Value) { return tanh(Value); }
			///<summary>Returns the hyperbolic tangent of a value.</summary>
			///<param name="Value">The value.</param>
			static double Tanh(double Value) { return tanh(Value); }
			///<summary>Returns the hyperbolic tangent of a value.</summary>
			///<param name="Complex">The value.</param>
			static Draw8::Complex^ Tanh(Complex^ Complex) { return ref new Draw8::Complex(std::tanh(Complex->AsComplex())); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the arc hyperbolic tangent of a value.</summary>
			///<param name="Value">The value.</param>
			static float Atanh(float Value) { return std::atanh(std::complex<float>(Value, 0)).real(); }
			///<summary>Returns the arc hyperbolic tangent of a value.</summary>
			///<param name="Value">The value.</param>
			static double Atanh(double Value) { return std::atanh(std::complex<double>(Value, 0)).real(); }
			///<summary>Returns the arc hyperbolic tangent of a complex value.</summary>
			///<param name="Complex">The value.</param>
			static Draw8::Complex^ Atanh(Complex^ Complex) { return ref new Draw8::Complex(std::atanh(Complex->AsComplex())); }
			
			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the absolute value a number.</summary>
			///<param name="Value">The value.</param>
			static float Abs(float Value) { return abs(Value); }
			///<summary>Returns the absolute value a number.</summary>
			///<param name="Value">The value.</param>
			static double Abs(double Value) { return abs(Value); }
			///<summary>Returns the absolute value a complex (its length).</summary>
			///<param name="Value">The value.</param>
			static float Abs(Complex^ Complex) { return std::abs((Complex->AsComplex())); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the exponential of a value (e^Value).</summary>
			///<param name="Value">The value.</param>
			static float Exp(float Value) { return exp(Value); }
			///<summary>Returns the exponential of a value (e^Value).</summary>
			///<param name="Value">The value.</param>
			static double Exp(double Value) { return exp(Value); }
			///<summary>Returns the exponential of a complex value (e^Value).</summary>
			///<param name="Complex">The value.</param>
			static Complex^ Exp(Complex^ Complex) { return ref new Draw8::Complex(exp((Complex->AsComplex()))); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the logarithm (base e) of a value (e^Value).</summary>
			///<param name="Value">The value.</param>
			static float Log(float Value) { return log(Value); }
			///<summary>Returns the logarithm (base e) of a value (e^Value).</summary>
			///<param name="Value">The value.</param>
			static double Log(double Value) { return log(Value); }
			///<summary>Returns the logarithm (base e) of a value (e^Value).</summary>
			///<param name="Complex">The value.</param>
			static Complex^ Log(Complex^ Complex) { return ref new Draw8::Complex(log((Complex->AsComplex()))); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the logarithm (base b) of a value (e^Value).</summary>
			///<param name="Value">The value.</param>
			///<param name="Base">The base.</param>
			static float Log(float Value, float Base) { return log(Value) / log(Base); }
			///<summary>Returns the logarithm (base b) of a value (e^Value).</summary>
			///<param name="Value">The value.</param>
			///<param name="Base">The base.</param>
			static double Log(double Value, double Base) { return log(Value) / log(Base); }
			///<summary>Returns the logarithm (base b) of a value (e^Value).</summary>
			///<param name="Complex">The value.</param>
			///<param name="Base">The base.</param>
			static Complex^ Log(Complex^ Complex, Draw8::Complex^ Base) { return ref new Draw8::Complex(log(Complex->AsComplex()) / log(Base->AsComplex())); }
			
			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the logarithm (base 10) of a value (e^Value).</summary>
			///<param name="Value">The value.</param>
			static float Log10(float Value) { return log10(Value); }
			///<summary>Returns the logarithm (base 10) of a value (e^Value).</summary>
			///<param name="Value">The value.</param>
			static double Log10(double Value) { return log10(Value); }
			///<summary>Returns the logarithm (base 10) of a value (e^Value).</summary>
			///<param name="Complex">The value.</param>
			static Complex^ Log10(Complex^ Complex) { return ref new Draw8::Complex(log10((Complex->AsComplex()))); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the logarithm (base 2) of a value (e^Value).</summary>
			///<param name="Value">The value.</param>
			static float Log2(float Value) { return log(Value) / log(2.f); }
			///<summary>Returns the logarithm (base 2) of a value (e^Value).</summary>
			///<param name="Value">The value.</param>
			static double Log2(double Value) { return log(Value) / log(2); }
			///<summary>Returns the logarithm (base 2) of a value (e^Value).</summary>
			///<param name="Complex">The value.</param>
			static Complex^ Log2(Complex^ Complex) { return ref new Draw8::Complex(log(Complex->AsComplex()) / logf(2)); }
			
			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns Value power Exponent.</summary>
			///<param name="Value">The value.</param>
			///<param name="Exponent">The exponent.</param>
			static float Pow(float Value, float Exponent) { return pow(Value, Exponent); }
			///<summary>Returns Value power Exponent.</summary>
			///<param name="Value">The value.</param>
			///<param name="Exponent">The exponent.</param>
			static double Pow(double Value, double Exponent) { return pow(Value, Exponent); }
			///<summary>Returns Value power Exponent.</summary>
			///<param name="Value">The value.</param>
			///<param name="Exponent">The exponent.</param>
			static Complex^ Pow(Complex^ Value, Complex^ Exponent) { return ref new Draw8::Complex(pow(Value->AsComplex(), Exponent->AsComplex())); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the maximum of two values.</summary>
			///<param name="X">The first value.</param>
			///<param name="Y">The other value.</param>
			static float Max(float X, float Y) { return max(X, Y); }
			///<summary>Returns the maximum of two values.</summary>
			///<param name="X">The first value.</param>
			///<param name="Y">The other value.</param>
			static double Max(double X, double Y) { return max(X, Y); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the minimum of two values.</summary>
			///<param name="X">The first value.</param>
			///<param name="Y">The other value.</param>
			static float Min(float X, float Y) { return min(X, Y); }
			///<summary>Returns the minimum of two values.</summary>
			///<param name="X">The first value.</param>
			///<param name="Y">The other value.</param>
			static double Min(double X, double Y) { return min(X, Y); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the square root of a value.</summary>
			///<param name="Value">The value.</param>
			static float SquareRoot(float Value) { return sqrt(Value); }
			///<summary>Returns the square root of a value.</summary>
			///<param name="Value">The value.</param>
			static double SquareRoot(double Value) { return sqrt(Value); }
			///<summary>Returns the square root of a value.</summary>
			///<param name="Complex">The value.</param>
			static Complex^ SquareRoot(Complex^ Complex) { return ref new Draw8::Complex(sqrt((Complex->AsComplex()))); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the square root of a value.</summary>
			///<param name="Value">The value.</param>
			static float Sqrt(float Value) { return sqrt(Value); }
			///<summary>Returns the square root of a value.</summary>
			///<param name="Value">The value.</param>
			static double Sqrt(double Value) { return sqrt(Value); }
			///<summary>Returns the square root of a value.</summary>
			///<param name="Complex">The value.</param>
			static Complex^ Sqrt(Complex^ Complex) { return ref new Draw8::Complex(sqrt((Complex->AsComplex()))); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the cube root of a value.</summary>
			///<param name="Value">The value.</param>
			static float Cbrt(float Value) { return pow(Value, 1.f/3.f); }
			///<summary>Returns the cube root of a value.</summary>
			///<param name="Value">The value.</param>
			static double Cbrt(double Value) { return pow(Value, 1.f / 3.f); }
			///<summary>Returns the cube root of a value.</summary>
			///<param name="Complex">The value.</param>
			static Complex^ Cbrt(Complex^ Complex) { return ref new Draw8::Complex(pow((Complex->AsComplex()), 1.f / 3.f)); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the cube root of a value.</summary>
			///<param name="Value">The value.</param>
			static float CubeRoot(float Value) { return pow(Value, 1.f / 3.f); }
			///<summary>Returns the cube root of a value.</summary>
			///<param name="Value">The value.</param>
			static double CubeRoot(double Value) { return pow(Value, 1.f / 3.f); }
			///<summary>Returns the cube root of a value.</summary>
			///<param name="Complex">The value.</param>
			static Complex^ CubeRoot(Complex^ Complex) { return ref new Draw8::Complex(pow((Complex->AsComplex()), 1.f / 3.f)); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the Order-th root of Value.</summary>
			///<param name="Value">The value.</param>
			///<param name="Order">The order of the root.</param>
			static float Root(float Value, float Order) { return pow(Value, 1.f / Order); }
			///<summary>Returns the Order-th root of Value.</summary>
			///<param name="Value">The value.</param>
			///<param name="Order">The order of the root.</param>
			static double Root(double Value, double Order) { return pow(Value, 1. / Order); }
			
			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the hypothenuse of a right triangle with X and Y as sides.</summary>
			///<param name="X">The X side.</param>
			///<param name="Y">The Y side.</param>
			static float Hypothenuse(float X, float Y) { return hypotf(X, Y); }
			///<summary>Returns the hypothenuse of a right triangle with X and Y as sides.</summary>
			///<param name="X">The X side.</param>
			///<param name="Y">The Y side.</param>
			static double Hypothenuse(double X, double Y) { return hypot(X, Y); }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the squared hypothenuse of a right triangle with X and Y as sides.</summary>
			///<param name="X">The X side.</param>
			///<param name="Y">The Y side.</param>
			static float SquaredHypothenuse(float X, float Y) { return (X*X, Y*Y); }
			///<summary>Returns the squared hypothenuse of a right triangle with X and Y as sides.</summary>
			///<param name="X">The X side.</param>
			///<param name="Y">The Y side.</param>
			static double SquaredHypothenuse(double X, double Y) { return (X*X, Y*Y); }

			
			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the sign of a value : 0 if null, 1 if positive and -1 if negative.</summary>
			///<param name="X">The value.</param>
			static float Sign(float X) { return X != 0 ? X / abs(X) : 0; }
			///<summary>Returns the sign of a value : 0 if null, 1 if positive and -1 if negative.</summary>
			///<param name="X">The value.</param>
			static double Sign(double X) { return X != 0 ? X / abs(X) : 0; }

			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the rounded-down value.</summary>
			///<param name="Value">The value.</param>
			static float Floor(float Value) { return floor(Value); }
			///<summary>Returns the rounded-down value.</summary>
			///<param name="Value">The value.</param>
			static double Floor(double Value) { return floor(Value); }
			
			[Windows::Foundation::Metadata::DefaultOverloadAttribute]
			///<summary>Returns the rounded-up value.</summary>
			///<param name="Value">The value.</param>
			static float Ceil(float Value) { return ceil(Value); }
			///<summary>Returns the rounded-up value.</summary>
			///<param name="Value">The value.</param>
			static double Ceil(double Value) { return ceil(Value); }

	};
}

#endif
