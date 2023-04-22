//***************************************************************************//
//* File Name: quaternion.hpp                                               *//
//* Author:    Tom Portegys, portegys@ilstu.edu                             *//
//* Date Made: 07/25/02                                                     *//
//* File Desc: Class declaration representing a quaternion.                 *//
//* Rev. Date:                                                              *//
//* Rev. Desc:                                                              *//
//*                                                                         *//
//***************************************************************************//

#ifndef __QUATERNION_HPP__
#define __QUATERNION_HPP__

#include "vector.hpp"

class cQuaternion
{
public:

   // Quaternion.
   float m_quat[4];

   // Constructors.
   cQuaternion();
   cQuaternion(float quat[4]);

   // Initialize.
   void init(float quat[4]);

   // Clear.
   void clear();

   // Vector operations.
   void vzero(float *);
   void vset(float *, float, float, float);
   void vsub(const float *, const float *, float *);
   void vcopy(const float *, float *);
   void vcross(const float *, const float *, float *);
   float vlength(const float *);
   void vscale(float *, float);
   void vnormal(float *);
   float vdot(const float *, const float *);
   void vadd(const float *, const float *, float *);

   // Add quaternions.
   void add_quats(cQuaternion& q1, cQuaternion& q2, cQuaternion& dest);

   // Multiply quaternions.
   void mult_quats(cQuaternion& q1, cQuaternion& q2, cQuaternion& dest);

   // Normalize a quaternion.
   void normalize_quat();

   // Build a rotation matrix given a quaternion rotation.
   void build_rotmatrix(float m[4][4]);

   // Load an axis-angle rotation into quaternion.
   void loadRotation(float angle, float *axis);

   // Merge an axis-angle rotation into quaternion.
   void mergeRotation(float angle, float *axis);

   // Make quaternion from Euler angles.
   void makeQFromEulerAngles(float pitch, float yaw, float roll);

   // Get Euler angles from quaternion.
   void getEulerAngles(float& pitch, float& yaw, float& roll);

   // Clone.
   cQuaternion *clone();

   // Load and save.
   void load(FILE *fp);
   void save(FILE *fp);

private:

   int m_normalCount;                             // Normalization counter.
};
#endif
