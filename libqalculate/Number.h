/*
    Qalculate (library)

    Copyright (C) 2003-2007, 2008, 2016  Hanna Knutsson (hanna.knutsson@protonmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/

#ifndef NUMBER_H
#define NUMBER_H

#include <libqalculate/includes.h>

#include <gmp.h>
#include <mpfr.h>

/** @file */

#define EQUALS_PRECISION_DEFAULT 	-1
#define EQUALS_PRECISION_LOWEST		-2
#define EQUALS_PRECISION_HIGHEST	-3

typedef enum {
	NUMBER_TYPE_RATIONAL,
	NUMBER_TYPE_FLOAT,
	NUMBER_TYPE_INFINITY,
	NUMBER_TYPE_PLUS_INFINITY,
	NUMBER_TYPE_MINUS_INFINITY
} NumberType;

typedef enum {
	INTEGER_TYPE_NONE,
	INTEGER_TYPE_SINT,
	INTEGER_TYPE_UINT,
	INTEGER_TYPE_ULONG,
	INTEGER_TYPE_SLONG,
	INTEGER_TYPE_SIZE
} IntegerType;

/// A number.
/**
* Can be rational, floating point, complex or infinite.
* Has arbitrary precision (uses Calculator::precision()) and infinitely large rational numbers.
* Implimented using GNU MP and MPFR.
 */
class Number {
	
	private:

	protected:
	
		void testInteger();
		bool testErrors(int error_level = 1) const;
		bool testFloatResult(bool allow_infinite_result = true, int error_level = 1, bool test_integer = true);
		void setPrecisionAndApproximateFrom(const Number &o);

		mpq_t r_value;
		mpfr_t f_value;
		
		Number *i_value;
		
		NumberType n_type;
		
		bool b_approx, b_imag;
		int i_precision;

	public:
	
		/**
		* Constructs a number initialized as zero.
 		*/
		Number();
		/**
		* Constructs a number parsing a text string.
		*
		* @param number Text string to read number from.
		* @param po Options for parsing the text string.
 		*/
		Number(string number, const ParseOptions &po = default_parse_options);
		/**
		* Constructs a rational number.
		*
		* @param numerator
		* @param denominator
		* @param exp_10
 		*/
		Number(long int numerator, long int denominator = 1, long int exp_10 = 0);
		/**
		* Constructs a copy of a number.
 		*/
		Number(const Number &o);
		virtual ~Number();
		
		void set(string number, const ParseOptions &po = default_parse_options);
		void set(long int numerator, long int denominator = 1, long int exp_10 = 0, bool keep_precision = false, bool keep_imag = false);
		void setInfinity(bool keep_precision = false);
		void setPlusInfinity(bool keep_precision = false);
		void setMinusInfinity(bool keep_precision = false);
		void setFloat(double d_value);

		void setInternal(const mpz_t &mpz_value, bool keep_precision = false, bool keep_imag = false);
		void setInternal(mpz_srcptr mpz_value, bool keep_precision = false, bool keep_imag = false);
		void setInternal(const mpq_t &mpq_value, bool keep_precision = false, bool keep_imag = false);
		void setInternal(const mpz_t &mpz_num, const mpz_t &mpz_den, bool keep_precision = false, bool keep_imag = false);
		void setInternal(const mpfr_t &mpfr_value, bool merge_precision = false, bool keep_imag = false);

		void setImaginaryPart(const Number &o);
		void setImaginaryPart(long int numerator, long int denominator = 1, long int exp_10 = 0);
		void set(const Number &o, bool merge_precision = false, bool keep_imag = false);
		void clear(bool keep_precision = false);
		void clearReal();
		void clearImaginary();
		
		const mpq_t &internalRational() const;
		const mpfr_t &internalFloat() const;
		mpq_t &internalRational();
		mpfr_t &internalFloat();
		Number *internalImaginary() const;
		void markAsImaginaryPart(bool is_imag = true);
		const NumberType &internalType() const;
		bool setToFloatingPoint();
		
		double floatValue() const;
		/**
		* Converts a number to an integer. If the number does not represent an integer it will rounded using round().
		*
		* @param[out] overflow If overflow is non-null it will be set to true if the number was to large to fit the return type.
		* @return Resulting integer.
 		*/
		int intValue(bool *overflow = NULL) const;
		unsigned int uintValue(bool *overflow = NULL) const;
		long int lintValue(bool *overflow = NULL) const;
		unsigned long int ulintValue(bool *overflow = NULL) const;
		
		/** Returns true if the number is approximate.
		*
 		* @return true if the number is approximate.
 		*/
		bool isApproximate() const;
		/** Returns true if the number is a floating point number.
		*
 		* @return true if the number has an approximate representation.
 		*/
		bool isFloatingPoint() const;
		/** Defines the number as approximate or exact.
		*
 		* @param is_approximate If the number shall be regarded as approximate.
 		*/
		void setApproximate(bool is_approximate = true);
		
		/** Returns the.precision of the number.
		*
 		* @return Precision of the number or -1 if the number is exact or the precision has not been set.
 		*/
		int precision() const;
		void setPrecision(int prec);
		
		bool isUndefined() const;
		/** Returns true if the number is infinity, plus infinity or minus infinity.
		*
 		* @return true if the number is infinite.
 		*/
		bool isInfinite() const;
		/** Returns true if the number is infinity, if the number is plus or minus infinity (which is not known).
		*
 		* @return true if the number is infinity.
 		*/
		bool isInfinity() const;
		/** Returns true if the number is plus infinity.
		*
 		* @return true if the number is plus infinity.
 		*/
		bool isPlusInfinity() const;
		/** Returns true if the number is minus infinity.
		*
 		* @return true if the number is minus infinity.
 		*/
		bool isMinusInfinity() const;
		
		/** Returns the real part of the number if it is complex, or a copy if it is real.
		*
 		* @return true if the real part of a complex number.
 		*/
		Number realPart() const;
		/** Returns the imaginary part as real number of the number if it is complex, or zero if it is real.
		*
 		* @return true if the imaginary part of a complex number.
 		*/
		Number imaginaryPart() const;
		Number numerator() const;
		Number denominator() const;
		Number complexNumerator() const;
		Number complexDenominator() const;

		void operator = (const Number &o);
		void operator = (long int i);
		void operator -- (int);
		void operator ++ (int);
		Number operator - () const;
		Number operator * (const Number &o) const;
		Number operator / (const Number &o) const;
		Number operator + (const Number &o) const;
		Number operator - (const Number &o) const;
		Number operator ^ (const Number &o) const;
		Number operator * (long int i) const;
		Number operator / (long int i) const;
		Number operator + (long int i) const;
		Number operator - (long int i) const;
		Number operator ^ (long int i) const;
		Number operator && (const Number &o) const;
		Number operator || (const Number &o) const;
		Number operator ! () const;
		
		void operator *= (const Number &o);
		void operator /= (const Number &o);
		void operator += (const Number &o);
		void operator -= (const Number &o);
		void operator ^= (const Number &o);
		void operator *= (long int i);
		void operator /= (long int i);
		void operator += (long int i);
		void operator -= (long int i);
		void operator ^= (long int i);
		
		bool operator == (const Number &o) const;
		bool operator != (const Number &o) const;
		bool operator == (long int i) const;
		bool operator != (long int i) const;
		
		bool bitAnd(const Number &o);
		bool bitOr(const Number &o);
		bool bitXor(const Number &o);
		bool bitNot();
		bool bitEqv(const Number &o);
		bool shiftLeft(const Number &o);
		bool shiftRight(const Number &o);
		bool shift(const Number &o);
		
		bool hasRealPart() const;
		bool hasImaginaryPart() const;
		bool isComplex() const;
		bool isInteger(IntegerType integer_type = INTEGER_TYPE_NONE) const;
		Number integer() const;
		bool isRational() const;
		bool isReal() const;
		bool isFraction() const;
		bool isZero() const;
		bool isOne() const;
		bool isTwo() const;
		bool isI() const;
		bool isMinusI() const;
		bool isMinusOne() const;
		bool isNegative() const;
		bool isNonNegative() const;
		bool isPositive() const;
		bool isNonPositive() const;
		bool realPartIsNegative() const;
		bool realPartIsPositive() const;
		bool imaginaryPartIsNegative() const;
		bool imaginaryPartIsPositive() const;
		bool hasNegativeSign() const;
		bool hasPositiveSign() const;
		bool equalsZero() const;
		bool equals(const Number &o) const;
		bool equals(long int i) const;
		bool equalsApproximately(const Number &o, int prec) const;
		ComparisonResult compare(const Number &o) const;
		ComparisonResult compare(long int i) const;
		ComparisonResult compareApproximately(const Number &o, int prec = EQUALS_PRECISION_LOWEST) const;
		ComparisonResult compareImaginaryParts(const Number &o) const;
		ComparisonResult compareRealParts(const Number &o) const;
		bool isGreaterThan(const Number &o) const;
		bool isLessThan(const Number &o) const;
		bool isGreaterThanOrEqualTo(const Number &o) const;
		bool isLessThanOrEqualTo(const Number &o) const;
		bool isGreaterThan(long int i) const;
		bool isLessThan(long int i) const;
		bool isGreaterThanOrEqualTo(long int i) const;
		bool isLessThanOrEqualTo(long int i) const;
		bool isEven() const;
		bool numeratorIsGreaterThan(long int i) const;
		bool numeratorIsLessThan(long int i) const;
		bool numeratorEquals(long int i) const;
		bool denominatorIsGreaterThan(long int i) const;
		bool denominatorIsLessThan(long int i) const;
		bool denominatorEquals(long int i) const;
		bool denominatorIsGreater(const Number &o) const;
		bool denominatorIsLess(const Number &o) const;
		bool denominatorIsEqual(const Number &o) const;
		bool denominatorIsEven() const;
		bool denominatorIsTwo() const;
		bool numeratorIsEven() const;
		bool numeratorIsOne() const;
		bool numeratorIsMinusOne() const;
		bool isOdd() const;
		
		int integerLength() const;

		/** Add to the number (x+o).
		*
		* @param o Number to add.
 		* @return true if the operation was successful.
 		*/
		bool add(const Number &o);
		bool add(long int i);
		/** Subtracts from to the number (x-o).
		*
		* @param o Number to subtract.
 		* @return true if the operation was successful.
 		*/
		bool subtract(const Number &o);
		bool subtract(long int i);
		/** Multiply the number (x*o).
		*
		* @param o Number to multiply with.
 		* @return true if the operation was successful.
 		*/
		bool multiply(const Number &o);
		bool multiply(long int i);
		/** Divide the number (x/o).
		*
		* @param o Number to divide by.
 		* @return true if the operation was successful.
 		*/
		bool divide(const Number &o);
		bool divide(long int i);
		/** Invert the number (1/x).
		*
 		* @return true if the operation was successful.
 		*/
		bool recip();
		/** Raise the number (x^o).
		*
		* @param o Number to raise to.
		* @param try_exact If an exact solution should be tried first (might be slow).
		* @param use_real_root If true return the real root (if exists), otherwise return the principal root.
 		* @return true if the operation was successful.
 		*/
		bool raise(const Number &o, bool try_exact = true, bool real_root = false);
		bool sqrt();
		bool cbrt();
		bool root(const Number &o);
		bool allroots(const Number &o, vector<Number> &roots);
		/** Multiply the number with a power of ten (x*10^o).
		*
		* @param o Number to raise 10 by.
 		* @return true if the operation was successful.
 		*/
		bool exp10(const Number &o);
		/** Multiply the number with a power of two (x*2^o).
		*
		* @param o Number to raise 2 by.
 		* @return true if the operation was successful.
 		*/
		bool exp2(const Number &o);
		/** Set the number to ten raised by the number (10^x).
		*
 		* @return true if the operation was successful.
 		*/
		bool exp10();
		/** Set the number to two raised by the number (2^x).
		*
 		* @return true if the operation was successful.
 		*/
		bool exp2();
		/** Raise the number by two (x^2).
		*
 		* @return true if the operation was successful.
 		*/
		bool square();
		
		/** Negate the number (-x).
		*
 		* @return true if the operation was successful.
 		*/
		bool negate();
		void setNegative(bool is_negative);
		bool abs();
		bool signum();
		bool round(const Number &o);
		bool floor(const Number &o);
		bool ceil(const Number &o);
		bool trunc(const Number &o);
		bool mod(const Number &o);
		bool isIntegerDivisible(const Number &o) const;
		bool isqrt();
		bool round();
		bool floor();
		bool ceil();
		bool trunc();	
		bool frac();		
		bool rem(const Number &o);
		
		bool smod(const Number &o);
		bool irem(const Number &o);
		bool irem(const Number &o, Number &q);
		bool iquo(const Number &o);
		bool iquo(unsigned long int i);
		bool iquo(const Number &o, Number &r);

		int getBoolean() const;
		void toBoolean();
		void setTrue(bool is_true = true);
		void setFalse();
		void setLogicalNot();
		
		/** Set the number to e, the base of natural logarithm, calculated with the current default precision.
 		*/
		void e(bool use_cached_number = true);
		/** Set the number to pi, Archimede's constant, calculated with the current default precision.
 		*/
		void pi();
		/** Set the number to Catalan's constant, calculated with the current default precision.
 		*/
		void catalan();
		/** Set the number to Euler's constant, calculated with the current default precision.
 		*/
		void euler();
		/** Set the number to Riemann's zeta with the number as integral point. The number must be an integer greater than one.
		*
 		* @return true if the calculation was successful.
 		*/
		bool zeta();
		
		bool gamma();
		bool digamma();
		bool airy();
		bool erf();
		bool erfc();
		bool besselj(const Number &o);
		bool bessely(const Number &o);
		
		bool sin();
		bool asin();
		bool sinh();
		bool asinh();
		bool cos();
		bool acos();
		bool cosh();
		bool acosh();
		bool tan();
		bool atan();
		bool atan2(const Number &o);
		bool arg();
		bool tanh();
		bool atanh();
		bool ln();
		bool log(const Number &o);
		bool exp();
		bool lambertW();
		bool gcd(const Number &o);
		bool lcm(const Number &o);
		
		bool factorial();
		bool multiFactorial(const Number &o);
		bool doubleFactorial();
		bool binomial(const Number &m, const Number &k);
		bool factorize(vector<Number> &factors);
		
		void rand();
		void intRand(const Number &ceil);
	
		bool add(const Number &o, MathOperation op); 

		string printNumerator(int base = 10, bool display_sign = true, BaseDisplay base_display = BASE_DISPLAY_NORMAL, bool lower_case = false) const;
		string printDenominator(int base = 10, bool display_sign = true, BaseDisplay base_display = BASE_DISPLAY_NORMAL, bool lower_case = false) const;
		string printImaginaryNumerator(int base = 10, bool display_sign = true, BaseDisplay base_display = BASE_DISPLAY_NORMAL, bool lower_case = false) const;
		string printImaginaryDenominator(int base = 10, bool display_sign = true, BaseDisplay base_display = BASE_DISPLAY_NORMAL, bool lower_case = false) const;

		string print(const PrintOptions &po = default_print_options, const InternalPrintStruct &ips = top_ips) const;
	
};

#endif
