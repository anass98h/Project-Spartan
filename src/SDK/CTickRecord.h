#pragma once

struct CTickRecord;

struct CValidTick {
    explicit operator CTickRecord() const;

    explicit operator bool() const noexcept {
        return m_flSimulationTime > 0.f;
    }

    float m_flPitch = 0.f;
    float m_flYaw = 0.f;
    float m_flSimulationTime = 0.f;
    C_BasePlayer* m_pEntity = nullptr;
};

struct CTickRecord {
    CTickRecord() {
    }

    CTickRecord( C_BasePlayer* player ) {
        m_flLowerBodyYawTarget = *player->GetLowerBodyYawTarget();
        m_angEyeAngles = *player->GetEyeAngles();
        m_flCycle = player->GetCycle();
        m_flSimulationTime = player->GetSimulationTime();
        m_nSequence = player->GetSequence();
        m_vecOrigin = player->GetVecOrigin();
        m_vecVelocity = player->GetVelocity();
        m_flPoseParameter = player->GetPoseParameter();
        headPos = player->GetBonePosition( ( int ) Bone::BONE_HEAD );
        tickcount = 0;
    }

    explicit operator bool() const noexcept {
        return m_flSimulationTime > 0.f;
    }

    bool operator>( const CTickRecord& others ) {
        return ( m_flSimulationTime > others.m_flSimulationTime );
    }

    bool operator>=( const CTickRecord& others ) {
        return ( m_flSimulationTime >= others.m_flSimulationTime );
    }

    bool operator<( const CTickRecord& others ) {
        return ( m_flSimulationTime < others.m_flSimulationTime );
    }

    bool operator<=( const CTickRecord& others ) {
        return ( m_flSimulationTime <= others.m_flSimulationTime );
    }

    bool operator==( const CTickRecord& others ) {
        return ( m_flSimulationTime == others.m_flSimulationTime );
    }

    /* 	float m_flLowerBodyYawTarget = 0.f;
	QAngle m_angEyeAngles = QAngle(0, 0, 0);
	float m_flCycle = 0.f;
	float m_flSimulationTime = 0.f;
	int m_nSequence = 0;
	Vector m_vecOrigin = Vector(0, 0, 0);
	Vector m_vecAbsOrigin = Vector(0, 0, 0);
	Vector m_vecVelocity = Vector(0, 0, 0);
	std::array<float, 24> m_flPoseParameter = {};
	QAngle m_angAbsAngles = QAngle(0, 0, 0);
	CValidTick validtick;
	int tickcount = 0;*/

    float m_flLowerBodyYawTarget = 0.f;
    QAngle m_angEyeAngles = QAngle( 0, 0, 0 );
    float m_flCycle = 0.f;
    float m_flSimulationTime = 0.f;
    int m_nSequence = 0;
    Vector m_vecOrigin = Vector( 0, 0, 0 );
    Vector m_vecVelocity = Vector( 0, 0, 0 );
    std::array<float, 24> m_flPoseParameter = {};
    Vector headPos;
    CValidTick validtick;
    int tickcount = 0;
};

inline CValidTick::operator CTickRecord() const {
    CTickRecord rec( m_pEntity );
    rec.m_angEyeAngles.x = this->m_flPitch;
    rec.m_angEyeAngles.y = this->m_flYaw;
    rec.m_flSimulationTime = this->m_flSimulationTime;
    return rec;
}
