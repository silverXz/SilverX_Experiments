// xVector.h: interface for the CxVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined _XVECTOR_H_INCLUDED_
#define _XVECTOR_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cassert>
#include <cmath>
#include <iostream>


template<class Type, int Dim>
class CxVector  
{
private:

	Type vec[Dim];

public:
	CxVector()
	{
		for ( int i=0; i<Dim; ++i )
			vec[i] = Type(0);
	}

	// Constructor for 1-4 arguments.  Ugly, but the compiler *should*
	// optimize away the ifs and unroll the for...  This one's explicit.
	explicit CxVector(Type x, Type y = Type(0), Type z = Type(0), Type w = Type(0))
	{
		vec[0] = x;  
		if ( Dim > 1 )
			vec[1] = y;
		if ( Dim > 2 )
			vec[2] = z;
		if ( Dim > 3 )
			vec[3] = w;

		for ( int i=4; i<Dim; i++)
			vec[i] = Type(0);
	}
	
	// Constructor from anything that can be accessed using []
	// This one's pretty aggressive, so marked explicit
	template <class S> explicit CxVector(const S& x)
	{
		for (int i = 0; i < Dim; i++)
			vec[i] = x[i]; 
	}

	// No destructor or assignment operator needed
	
	// Array reference and conversion to pointer;
	const Type &operator [] (int i) const { assert( 0<=i && i<Dim ); return vec[i]; }
	      Type &operator [] (int i)       { assert( 0<=i && i<Dim ); return vec[i]; }

	operator const Type* () const{ return vec; }
	operator const Type* ()      { return vec; }
	      operator Type* ()	     { return vec; }

	// Member operators
	CxVector<Type, Dim>& operator+=(const CxVector<Type, Dim>& rVec)
	{
		for (int i=0; i<Dim; ++i)
			vec[i] += rVec[i];
		return *this;
	}

	CxVector<Type, Dim>& operator-=(const CxVector<Type, Dim>& rVec)
	{
		for (int i=0; i<Dim; ++i)
			vec[i] -= rVec[i];
		return *this;
	}
	CxVector<Type, Dim>& operator*=(const CxVector<Type, Dim>& rVec)
	{
		for (int i=0; i<Dim; ++i)
			vec[i] *= rVec[i];
		return *this;
	}

	CxVector<Type, Dim>& operator*=(const Type& coef)
	{
		for (int i=0; i<Dim; ++i)
			vec[i] *= coef;
		return *this;
	}

	CxVector<Type, Dim>& operator/=(const CxVector<Type, Dim>& rVec)
	{
		for (int i=0; i<Dim; ++i)
			vec[i] /= rVec[i];
		return *this;
	}
	CxVector<Type, Dim>& operator/=(const Type& coef)
	{
		for (int i=0; i<Dim; ++i)
			vec[i] /= coef;
		return *this;
	}	
	
	
	      Type *begin()      { return &(vec[0]);     }
	const Type *begin()const { return &(vec[0]);     }
	      Type *end()        { return begin() + Dim; }
	const Type *end()const   { return begin() + Dim; }
	void clear() { for (int i = 0; i < Dim; i++) vec[i] = Type(0); }
	size_t size()const { return Dim;}
	


	// 
	const Type length2()const { return (*this)^(*this);}
	const Type length()const  { return sqrt(length2());}

	const Type dist2(const CxVector<Type, Dim>& v)const
	{
		Type d2 = (v[0] - vec[0])*(v[0] - vec[0]);
		for (int i=1; i<Dim; ++i )
			d2 += (v[i] - vec[i])*(v[i] - vec[i]);
		return d2;
	}
	const Type dist(const CxVector<Type, Dim>& v)const{ return sqrt(dist2(v));}

	void normalize()
	{
		Type len = length();
		if ( len <= Type(0) )
		{
			vec[0] = Type(1);
			for (int i=1; i<Dim; ++i )
				vec[i] = Type(0);
			return;
		}
		len = Type(1)/len;
		for (int i=0; i<Dim; i++)
			vec[i] *= len;
	}

};


// Nonmember operators that take two Vecs
template <class Type, int Dim>
static inline const CxVector<Type, Dim> operator+(const CxVector<Type, Dim>& v1, 
												  const CxVector<Type, Dim>& v2)
{
	return CxVector<Type, Dim>(v1) += v2;
}


template <class Type, int Dim>
static inline const CxVector<Type, Dim> operator-(const CxVector<Type, Dim>& v1, 
												  const CxVector<Type, Dim>& v2)
{
	return CxVector<Type, Dim>(v1) -= v2;
}

template <class Type, int Dim>
static inline const CxVector<Type, Dim> operator*(const CxVector<Type, Dim>& v1, 
												  const CxVector<Type, Dim>& v2)
{
	return CxVector<Type, Dim>(v1) *= v2;
}

template <class Type, int Dim>
static inline const CxVector<Type, Dim> operator/(const CxVector<Type, Dim>& v1, 
												  const CxVector<Type, Dim>& v2)
{
	return CxVector<Type, Dim>(v1) /= v2;
}

// dot product;
template <class Type, int Dim>
static inline const Type operator^(const CxVector<Type, Dim>& v1, 
												  const CxVector<Type, Dim>& v2)
{
	Type sum = v1[0] * v2[0];
	for (int i=1; i<Dim; ++i)
		sum += v1[i] * v2[i];
	return sum;
}
#define DOT ^

// cross product;
template <class Type, int Dim>
static inline const CxVector<Type, Dim> operator%(const CxVector<Type, Dim>& v1, 
												  const CxVector<Type, Dim>& v2)
{	
	return CxVector<Type, Dim>(v1[1]*v2[2] - v1[2]*v2[1],
					           v1[2]*v2[0] - v1[0]*v2[2],
					           v1[0]*v2[1] - v1[1]*v2[0]);
}
#define CROSS %


// Component-wise equality and inequality (#include the usual caveats
// about comparing floats for equality...)
template <class Type, int Dim>
static inline bool operator==(const CxVector<Type, Dim>& v1, 
												  const CxVector<Type, Dim>& v2)
{
	for (int i=0; i<Dim; ++i)
		if ( v1[i] != v2[i] )
			return false;
	return true;
}


template <class Type, int Dim>
static inline bool operator!=(const CxVector<Type, Dim>& v1, 
							  const CxVector<Type, Dim>& v2)
{
	for (int i=0; i<Dim; ++i)
		if ( v1[i] != v2[i] )
			return true;
		return false;
}

// Unary operators
template <class Type, int Dim>
static inline const CxVector<Type, Dim>&operator + (const CxVector<Type, Dim>& v)
{
	return v;
}

template <class Type, int Dim>
static inline const CxVector<Type, Dim> operator-(const CxVector<Type, Dim>& v)
{
	CxVector<Type, Dim> result(v);
	for ( int i=0; i<Dim; ++i )
		result[i] = -result[i];
	return result;
}

template <class Type, int Dim>
static inline bool operator ! (const CxVector<Type, Dim>& v)
{
	for ( int i = 0; i<Dim; ++i )
		if (v[i]) 
			return false;
		return true;
}


// Vec/scalar operators
template <class Type, int Dim>
static inline const CxVector<Type, Dim>operator * (const Type &x, 
												   const CxVector<Type, Dim>& v)
{
	CxVector<Type, Dim> result(v);
	for ( int i=0; i<Dim; ++i )
		result[i] = x*result[i];
	return result;
}

template<class Type, int Dim>
static inline const CxVector<Type, Dim>operator*(const CxVector<Type, Dim>&v,
												 const Type& x)
{
	return CxVector<Type, Dim>(v) *= x;
}

template <class Type, int Dim>
static inline const CxVector<Type, Dim>operator/(const Type &x, 
												 const CxVector<Type, Dim>& v)
{
	CxVector<Type, Dim> result(v);
	for ( int i=0; i<Dim; ++i )
		result[i] = x/result[i];
	return result;
}

template<class Type, int Dim>
static inline const CxVector<Type, Dim>operator/(const CxVector<Type, Dim>&v,
												 const Type& x)
{
	return CxVector<Type, Dim>(v) /= x;
}

// iostream operators
template <class Type, int Dim>
static inline std::ostream &operator<<(std::ostream& os, const CxVector<Type, Dim>& v)
{
	os << "(";
	for (int i = 0; i < Dim-1; i++)
		os << v[i] << ", ";
	return os << v[Dim-1] << ")";
}

template <class Type, int Dim>
static inline std::istream&operator>>(std::istream& is, CxVector<Type, Dim>& v)
{
	char c1 = 0, c2 = 0, c3 = 0;
	is >> c1;
	if ( c1 == '(' || c1 == '[' ) 
	{
		is >> v[0] >> std::ws >> c2;
		for (int i = 1; i < Dim; i++)
			if (c2 == ',')
				is >> v[i] >> std::ws >> c2;
	}
	
	if ( c1 == '(' && c3 != ')' )
		is.setstate(std::ios::failbit);
	else if (c1 == '[' && c3 != ']')
		is.setstate(std::ios::failbit);
	
	return is;
}

template <class Type, int Dim>
static inline void swap(const CxVector<Type, Dim> &v1, const CxVector<Type, Dim>& v2)
{
	for (int i = 0; i < Dim; i++)
		swap(v1[i], v2[i]);
}

template <class T>
static inline T sqr(const T &x)
{
	return x*x;
}

// Sign of a scalar
template <class T>
static inline T sgn(const T &x)
{
	return (x < T(0)) ? T(-1) : T(1);
}

typedef CxVector<float, 3> Vec3f;

#endif // !defined(_XVECTOR_H_INCLUDED_)
