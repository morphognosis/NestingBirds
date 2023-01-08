#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include "common.h"

//------------------------------------------------------------------------//
// Misc. Constants
//------------------------------------------------------------------------//

float const pi  = 3.14159265f;
float const tol = 0.0000000001f;                  // float type tolerance

//------------------------------------------------------------------------//
// Misc. Functions
//------------------------------------------------------------------------//
inline float DegreesToRadians(float deg)
{
   return(deg * pi / 180.0f);
}


inline float RadiansToDegrees(float rad)
{
   return(rad * 180.0f / pi);
}


//------------------------------------------------------------------------//
// Vector Class and vector functions
//------------------------------------------------------------------------//
class Vector
{
public:
   float x;
   float y;
   float z;

   Vector(void);
   Vector(float xi, float yi, float zi);

   float Magnitude(void);
   void Normalize(void);
   void Normalize(float);
   void Reverse(void);

   float Distance(Vector);
   float SquareDistance(Vector);
   void Zero(void);

   Vector& operator+=(Vector u);                  // vector addition
   Vector& operator-=(Vector u);                  // vector subtraction
   Vector& operator*=(float s);                   // scalar multiply
   Vector& operator/=(float s);                   // scalar divide

   Vector operator-(void);
};

inline Vector operator+(Vector u, Vector v);
inline Vector operator-(Vector u, Vector v);
inline Vector operator^(Vector u, Vector v);
inline float operator *(Vector u, Vector v);
inline Vector operator *(float s, Vector u);
inline Vector operator *(Vector u, float s);
inline Vector operator/(Vector u, float s);
inline float TripleScalarProduct(Vector u, Vector v, Vector w);

inline Vector::Vector(void)
{
   x = 0;
   y = 0;
   z = 0;
}


inline Vector::Vector(float xi, float yi, float zi)
{
   x = xi;
   y = yi;
   z = zi;
}


inline float Vector::Magnitude(void)
{
   return((float)sqrt(x * x + y * y + z * z));
}


inline void Vector::Normalize(void)
{
   float m = (float)sqrt(x * x + y * y + z * z);

   if (m <= tol)
   {
      m = 1;
   }
   x /= m;
   y /= m;
   z /= m;

   if (fabs(x) < tol)
   {
      x = 0.0f;
   }
   if (fabs(y) < tol)
   {
      y = 0.0f;
   }
   if (fabs(z) < tol)
   {
      z = 0.0f;
   }
}


inline void Vector::Normalize(float n)
{
   float m = (float)sqrt(x * x + y * y + z * z);

   if (m <= tol)
   {
      m = 1;
   }
   m  = n / m;
   x *= m;
   y *= m;
   z *= m;

   if (fabs(x) < tol)
   {
      x = 0.0f;
   }
   if (fabs(y) < tol)
   {
      y = 0.0f;
   }
   if (fabs(z) < tol)
   {
      z = 0.0f;
   }
}


inline void Vector::Reverse(void)
{
   x = -x;
   y = -y;
   z = -z;
}


inline float Vector::Distance(Vector v)
{
   float dx, dy, dz;

   dx = x - v.x;
   dy = y - v.y;
   dz = z - v.z;
   return((float)sqrt(dx * dx + dy * dy + dz * dz));
}


inline float Vector::SquareDistance(Vector v)
{
   float dx, dy, dz;

   dx = x - v.x;
   dy = y - v.y;
   dz = z - v.z;
   return((float)(dx * dx + dy * dy + dz * dz));
}


inline void Vector::Zero(void)
{
   x = 0.0;
   y = 0.0;
   z = 0.0;
}


inline Vector& Vector::operator+=(Vector u)
{
   x += u.x;
   y += u.y;
   z += u.z;
   return(*this);
}


inline Vector& Vector::operator-=(Vector u)
{
   x -= u.x;
   y -= u.y;
   z -= u.z;
   return(*this);
}


inline Vector& Vector::operator*=(float s)
{
   x *= s;
   y *= s;
   z *= s;
   return(*this);
}


inline Vector& Vector::operator/=(float s)
{
   x /= s;
   y /= s;
   z /= s;
   return(*this);
}


inline Vector Vector::operator-(void)
{
   return(Vector(-x, -y, -z));
}


inline Vector operator+(Vector u, Vector v)
{
   return(Vector(u.x + v.x, u.y + v.y, u.z + v.z));
}


inline Vector operator-(Vector u, Vector v)
{
   return(Vector(u.x - v.x, u.y - v.y, u.z - v.z));
}


// Vector cross product (u cross v)
inline Vector operator^(Vector u, Vector v)
{
   return(Vector(u.y * v.z - u.z * v.y,
                 -u.x * v.z + u.z * v.x,
                 u.x * v.y - u.y * v.x));
}


// Vector dot product
inline float operator *(Vector u, Vector v)
{
   return(u.x * v.x + u.y * v.y + u.z * v.z);
}


inline Vector operator *(float s, Vector u)
{
   return(Vector(u.x * s, u.y * s, u.z * s));
}


inline Vector operator *(Vector u, float s)
{
   return(Vector(u.x * s, u.y * s, u.z * s));
}


inline Vector operator/(Vector u, float s)
{
   return(Vector(u.x / s, u.y / s, u.z / s));
}


// triple scalar product (u dot (v cross w))
inline float TripleScalarProduct(Vector u, Vector v, Vector w)
{
   return(float((u.x * (v.y * w.z - v.z * w.y)) +
                (u.y * (-v.x * w.z + v.z * w.x)) +
                (u.z * (v.x * w.y - v.y * w.x))));
   //return u*(v^w);
}


#endif
