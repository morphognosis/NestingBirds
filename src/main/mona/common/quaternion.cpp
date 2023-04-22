//***************************************************************************//
//* File Name: quaternion.cpp                                               *//
//* Author:    Tom Portegys, portegys@ilstu.edu                             *//
//* Date Made: 07/25/02                                                     *//
//* File Desc: Implementation details representing a quaternion.            *//
//* Rev. Date:                                                              *//
//* Rev. Desc:                                                              *//
//*                                                                         *//
//***************************************************************************//

#include "quaternion.hpp"

// Constructors.
cQuaternion::cQuaternion()
{
   m_quat[0]     = 0.0;
   m_quat[1]     = 0.0;
   m_quat[2]     = 0.0;
   m_quat[3]     = 1.0;
   m_normalCount = 0;
}


cQuaternion::cQuaternion(float quat[4])
{
   init(quat);
   m_normalCount = 0;
}


// Initialize.
void cQuaternion::init(float quat[4])
{
   m_quat[0] = quat[0];
   m_quat[1] = quat[1];
   m_quat[2] = quat[2];
   m_quat[3] = quat[3];
}


// Clear.
void cQuaternion::clear()
{
   m_quat[0]     = 0.0;
   m_quat[1]     = 0.0;
   m_quat[2]     = 0.0;
   m_quat[3]     = 1.0;
   m_normalCount = 0;
}


void cQuaternion::vzero(float *v)
{
   v[0] = 0.0;
   v[1] = 0.0;
   v[2] = 0.0;
}


void cQuaternion::vset(float *v, float x, float y, float z)
{
   v[0] = x;
   v[1] = y;
   v[2] = z;
}


void cQuaternion::vsub(const float *src1, const float *src2, float *dst)
{
   dst[0] = src1[0] - src2[0];
   dst[1] = src1[1] - src2[1];
   dst[2] = src1[2] - src2[2];
}


void cQuaternion::vcopy(const float *v1, float *v2)
{
   for (int i = 0; i < 3; i++)
   {
      v2[i] = v1[i];
   }
}


void cQuaternion::vcross(const float *v1, const float *v2, float *cross)
{
   float temp[3];

   temp[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
   temp[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
   temp[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);

   vcopy(temp, cross);
}


float cQuaternion::vlength(const float *v)
{
   return((float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]));
}


void cQuaternion::vscale(float *v, float div)
{
   v[0] *= div;
   v[1] *= div;
   v[2] *= div;
}


void cQuaternion::vnormal(float *v)
{
   vscale(v, 1.0f / vlength(v));
}


float cQuaternion::vdot(const float *v1, const float *v2)
{
   return(v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}


void cQuaternion::vadd(const float *src1, const float *src2, float *dst)
{
   dst[0] = src1[0] + src2[0];
   dst[1] = src1[1] + src2[1];
   dst[2] = src1[2] + src2[2];
}


/*
 * Add quaternions, normalizing periodically.
 */

#define NORMALFREQ    100

void cQuaternion::add_quats(cQuaternion& q1, cQuaternion& q2, cQuaternion& dest)
{
   float t1[4], t2[4], t3[4];
   float tf[4];

   vcopy(q1.m_quat, t1);
   vscale(t1, q2.m_quat[3]);
   vcopy(q2.m_quat, t2);
   vscale(t2, q1.m_quat[3]);
   vcross(q2.m_quat, q1.m_quat, t3);
   vadd(t1, t2, tf);
   vadd(t3, tf, tf);
   tf[3]          = q1.m_quat[3] * q2.m_quat[3] - vdot(q1.m_quat, q2.m_quat);
   dest.m_quat[0] = tf[0];
   dest.m_quat[1] = tf[1];
   dest.m_quat[2] = tf[2];
   dest.m_quat[3] = tf[3];

   if (++m_normalCount > NORMALFREQ)
   {
      m_normalCount = 0;
      dest.normalize_quat();
   }
}


/*
 * Multiply quaternions, normalizing periodically.
 */
void cQuaternion::mult_quats(cQuaternion& q1, cQuaternion& q2, cQuaternion& dest)
{
   dest.m_quat[3] = q2.m_quat[3] * q1.m_quat[3]
                    - q2.m_quat[0] * q1.m_quat[0]
                    - q2.m_quat[1] * q1.m_quat[1]
                    - q2.m_quat[2] * q1.m_quat[2];

   dest.m_quat[0] = q2.m_quat[3] * q1.m_quat[0]
                    + q2.m_quat[0] * q1.m_quat[3]
                    + q2.m_quat[1] * q1.m_quat[2]
                    - q2.m_quat[2] * q1.m_quat[1];

   dest.m_quat[1] = q2.m_quat[3] * q1.m_quat[1]
                    - q2.m_quat[0] * q1.m_quat[2]
                    + q2.m_quat[1] * q1.m_quat[3]
                    + q2.m_quat[2] * q1.m_quat[0];

   dest.m_quat[2] = q2.m_quat[3] * q1.m_quat[2]
                    + q2.m_quat[0] * q1.m_quat[1]
                    - q2.m_quat[1] * q1.m_quat[0]
                    + q2.m_quat[2] * q1.m_quat[3];

   if (++m_normalCount > NORMALFREQ)
   {
      m_normalCount = 0;
      dest.normalize_quat();
   }
}


/*
 * Quaternions always obey:  a^2 + b^2 + c^2 + d^2 = 1.0
 * If they don't add up to 1.0, dividing by their magnitude will
 * renormalize them.
 *
 * Note: See the following for more information on quaternions:
 *
 * - Shoemake, K., Animating rotation with quaternion curves, Computer
 *   Graphics 19, No 3 (Proc. SIGGRAPH'85), 245-254, 1985.
 * - Pletinckx, D., Quaternion calculus as a basic tool in computer
 *   graphics, The Visual Computer 5, 2-13, 1989.
 */
void cQuaternion::normalize_quat()
{
   int   i;
   float mag;

   mag = (m_quat[0] * m_quat[0] + m_quat[1] * m_quat[1] +
          m_quat[2] * m_quat[2] + m_quat[3] * m_quat[3]);
   for (i = 0; i < 4; i++)
   {
      m_quat[i] /= mag;
   }
}


/*
 * Build a rotation matrix, given a quaternion rotation.
 */
void cQuaternion::build_rotmatrix(float m[4][4])
{
   m[0][0] = 1.0f - 2.0f * (m_quat[1] * m_quat[1] + m_quat[2] * m_quat[2]);
   m[0][1] = 2.0f * (m_quat[0] * m_quat[1] - m_quat[2] * m_quat[3]);
   m[0][2] = 2.0f * (m_quat[2] * m_quat[0] + m_quat[1] * m_quat[3]);
   m[0][3] = 0.0f;

   m[1][0] = 2.0f * (m_quat[0] * m_quat[1] + m_quat[2] * m_quat[3]);
   m[1][1] = 1.0f - 2.0f * (m_quat[2] * m_quat[2] + m_quat[0] * m_quat[0]);
   m[1][2] = 2.0f * (m_quat[1] * m_quat[2] - m_quat[0] * m_quat[3]);
   m[1][3] = 0.0f;

   m[2][0] = 2.0f * (m_quat[2] * m_quat[0] - m_quat[1] * m_quat[3]);
   m[2][1] = 2.0f * (m_quat[1] * m_quat[2] + m_quat[0] * m_quat[3]);
   m[2][2] = 1.0f - 2.0f * (m_quat[1] * m_quat[1] + m_quat[0] * m_quat[0]);
   m[2][3] = 0.0f;

   m[3][0] = 0.0f;
   m[3][1] = 0.0f;
   m[3][2] = 0.0f;
   m[3][3] = 1.0f;
}


// Load axis-angle rotation into quaternion.
void cQuaternion::loadRotation(float angle, float *axis)
{
   vnormal(axis);
   vcopy(axis, m_quat);
   vscale(m_quat, (float)sin(angle / 2.0f));
   m_quat[3] = (float)cos(angle / 2.0f);
   normalize_quat();
}


// Merge an axis-angle rotation into quaternion.
void cQuaternion::mergeRotation(float angle, float *axis)
{
   cQuaternion q1, q2;

   q2.m_quat[0] = axis[0] * (float)sin(angle / 2.0f);
   q2.m_quat[1] = axis[1] * (float)sin(angle / 2.0f);
   q2.m_quat[2] = axis[2] * (float)sin(angle / 2.0f);
   q2.m_quat[3] = (float)cos(angle / 2.0f);
   q2.normalize_quat();
   q1.m_quat[0] = m_quat[0];
   q1.m_quat[1] = m_quat[1];
   q1.m_quat[2] = m_quat[2];
   q1.m_quat[3] = m_quat[3];
   mult_quats(q2, q1, *this);
   normalize_quat();
}


// Make quaternion from Euler angles.
void cQuaternion::makeQFromEulerAngles(float pitch, float yaw, float roll)
{
   // Re-orient internally.
   float iroll  = DegreesToRadians(pitch);
   float ipitch = DegreesToRadians(yaw);
   float iyaw   = DegreesToRadians(roll);

   float cyaw, cpitch, croll, syaw, spitch, sroll;
   float cyawcpitch, syawspitch, cyawspitch, syawcpitch;

   cyaw   = cos(0.5f * iyaw);
   cpitch = cos(0.5f * ipitch);
   croll  = cos(0.5f * iroll);
   syaw   = sin(0.5f * iyaw);
   spitch = sin(0.5f * ipitch);
   sroll  = sin(0.5f * iroll);

   cyawcpitch = cyaw * cpitch;
   syawspitch = syaw * spitch;
   cyawspitch = cyaw * spitch;
   syawcpitch = syaw * cpitch;

   m_quat[3] = (float)(cyawcpitch * croll + syawspitch * sroll);
   m_quat[0] = (float)(cyawcpitch * sroll - syawspitch * croll);
   m_quat[1] = (float)(cyawspitch * croll + syawcpitch * sroll);
   m_quat[2] = (float)(syawcpitch * croll - cyawspitch * sroll);
}


// Get Euler angles.
void cQuaternion::getEulerAngles(float& pitch, float& yaw, float& roll)
{
   float r11, r21, r31, r32, r33, r12, r13;
   float q00, q11, q22, q33;
   float tmp;

   q00 = m_quat[3] * m_quat[3];
   q11 = m_quat[0] * m_quat[0];
   q22 = m_quat[1] * m_quat[1];
   q33 = m_quat[2] * m_quat[2];

   r11 = q00 + q11 - q22 - q33;
   r21 = 2 * (m_quat[0] * m_quat[1] + m_quat[3] * m_quat[2]);
   r31 = 2 * (m_quat[0] * m_quat[2] - m_quat[3] * m_quat[1]);
   r32 = 2 * (m_quat[1] * m_quat[2] + m_quat[3] * m_quat[0]);
   r33 = q00 - q11 - q22 + q33;

   tmp = fabs(r31);
   if (tmp > 0.999999)
   {
      r12 = 2 * (m_quat[0] * m_quat[1] - m_quat[3] * m_quat[2]);
      r13 = 2 * (m_quat[0] * m_quat[2] + m_quat[3] * m_quat[1]);

      pitch = RadiansToDegrees(0.0f);
      yaw   = RadiansToDegrees((float)(-(pi / 2) * r31 / tmp));
      roll  = RadiansToDegrees((float)atan2(-r12, -r31 * r13));
   }
   else
   {
      pitch = RadiansToDegrees((float)atan2(r32, r33));
      yaw   = RadiansToDegrees((float)asin(-r31));
      roll  = RadiansToDegrees((float)atan2(r21, r11));
   }
}


// Clone quaternion.
cQuaternion *cQuaternion::clone()
{
   cQuaternion *q = new cQuaternion();

   assert(q != NULL);
   q->m_quat[0] = m_quat[0];
   q->m_quat[1] = m_quat[1];
   q->m_quat[2] = m_quat[2];
   q->m_quat[3] = m_quat[3];
   return(q);
}


// Load quaternion.
void cQuaternion::load(FILE *fp)
{
   for (int i = 0; i < 4; i++)
   {
      FREAD_FLOAT(&m_quat[i], fp);
   }
   FREAD_INT(&m_normalCount, fp);
}


// Save quaternion.
void cQuaternion::save(FILE *fp)
{
   for (int i = 0; i < 4; i++)
   {
      FWRITE_FLOAT(&m_quat[i], fp);
   }
   FWRITE_INT(&m_normalCount, fp);
}
