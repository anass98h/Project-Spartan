#pragma once

#include "../SDK/SDK.h"

namespace Math {
    void inline SinCos( float radians, float* sine, float* cosine );

    void AngleVectors( const QAngle& angles, Vector& forward );

    void NormalizeAngles( QAngle& angle );
    
    float ResNormalizeYaw( float yaw );

    void ClampAngles( QAngle& angle );

    void ClampY( int& y );

    float RoundFloat( float f );

    void CorrectMovement( QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove );

    float GetFov( const QAngle& viewAngle, const QAngle& aimAngle );

    float DotProduct( Vector& v1, const float* v2 );

    void VectorAngles( const Vector& forward, QAngle& angles );

    void VectorTransform( Vector& in1, const matrix3x4_t& in2, Vector& out );

    QAngle CalcAngle( Vector src, Vector dst );

    float float_rand( float min, float max );

    void CalcAngleYawOnly( Vector src, Vector dst, Vector& angles );

    void Normalize( Vector& vIn, Vector& vOut );

    void NormalizePitch( float& pitch );

    void NormalizeVector( Vector& vec );

    void NormalizeYaw( float& yaw );

    void VectorAngles2( Vector& vecForward, Vector& vecAngles );

    void AngleVectors2( Vector& qAngles, Vector& vecForward );

    void AngleVectors( const Vector& angles, Vector* forward, Vector* right, Vector* up );

    float ClampYaw( float val );
}
