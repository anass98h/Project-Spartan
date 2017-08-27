#pragma once

#ifndef NETWORKSTRINGTABLEDEFS_H
#define NETWORKSTRINGTABLEDEFS_H
#ifdef _WIN32
#pragma once
#endif

typedef int TABLEID;

#define INVALID_STRING_TABLE -1
const unsigned short INVALID_STRING_INDEX = (unsigned short )-1;

// table index is sent in log2(MAX_TABLES) bits
#define MAX_TABLES	32  // Table id is 4 bits



class INetworkStringTable;

typedef void (*pfnStringChanged)( void *object, INetworkStringTable *stringTable, int stringNumber, char const *newString, void const *newData );

//-----------------------------------------------------------------------------
// Purpose: Game .dll shared string table interfaces
//-----------------------------------------------------------------------------
class INetworkStringTable
{
public:

	virtual					~INetworkStringTable( void ) {};
		
	// Table Info
	virtual const char		*GetTableName( void ) const = 0;
	virtual TABLEID			GetTableId( void ) const = 0;
	virtual int				GetNumStrings( void ) const = 0;
	virtual int				GetMaxStrings( void ) const = 0;
	virtual int				GetEntryBits( void ) const = 0;

	// Networking
	virtual void			SetTick( int tick ) = 0;
	virtual bool			ChangedSinceTick( int tick ) const = 0;

	// Accessors (length -1 means don't change user data if string already exits)
	virtual int				AddString( bool bIsServer, const char *value, int length = -1, const void *userdata = 0 ) = 0; 

	virtual const char		*GetString( int stringNumber ) = 0;
	virtual void			SetStringUserData( int stringNumber, int length, const void *userdata ) = 0;
	virtual const void		*GetStringUserData( int stringNumber, int *length ) = 0;
	virtual int				FindStringIndex( char const *string ) = 0; // returns INVALID_STRING_INDEX if not found

	// Callbacks
	virtual void			SetStringChangedCallback( void *object, pfnStringChanged changeFunc ) = 0;
};

class INetworkStringTableContainer
{
public:
	
	virtual					~INetworkStringTableContainer( void ) {};
	
	// table creation/destruction
	virtual INetworkStringTable	*CreateStringTable( const char *tableName, int maxentries, int userdatafixedsize = 0, int userdatanetworkbits = 0 ) = 0;
	virtual void				RemoveAllTables( void ) = 0;
	
	// table infos
	
	virtual INetworkStringTable	*GetTable( TABLEID stringTable ) const = 0;
	virtual int					GetNumTables( void ) const = 0;

	virtual INetworkStringTable	*CreateStringTableEx( const char *tableName, int maxentries, int userdatafixedsize = 0, int userdatanetworkbits = 0, bool bIsFilenames = false ) = 0;
	virtual void				SetAllowClientSideAddString( INetworkStringTable *table, bool bAllowClientSideAddString ) = 0;
            
        INetworkStringTable* FindTable(const char *tableName)
        	{
        		typedef INetworkStringTable*(* oFindTable)(void*, const char*);
        		return getvfunc<oFindTable>(this, 3)(this, tableName);
        	}


};

#endif // NETWORKSTRINGTABLEDEFS_H


#if !defined( INETCHANNELINFO_H )
#define INETCHANNELINFO_H



#define FLOW_OUTGOING	0		
#define FLOW_INCOMING	1
#define MAX_FLOWS		2		// in & out

class INetChannelInfo
{
public:

	enum {
		GENERIC = 0,	// must be first and is default group
		LOCALPLAYER,	// bytes for local player entity update
		OTHERPLAYERS,	// bytes for other players update
		ENTITIES,		// all other entity bytes
		SOUNDS,			// game sounds
		EVENTS,			// event messages
		USERMESSAGES,	// user messages
		ENTMESSAGES,	// entity messages
		VOICE,			// voice data
		STRINGTABLE,	// a stringtable update
		MOVE,			// client move cmds
		STRINGCMD,		// string command
		SIGNON,			// various signondata
		TOTAL,			// must be last and is not a real group
	};

	virtual const char  *GetName(void) const = 0;	// get channel name
	virtual const char  *GetAddress(void) const = 0; // get channel IP address as string
	virtual float		GetTime(void) const = 0;	// current net time
	virtual float		GetTimeConnected(void) const = 0;	// get connection time in seconds
	virtual int			GetBufferSize(void) const = 0;	// netchannel packet history size
	virtual int			GetDataRate(void) const = 0; // send data rate in byte/sec

	virtual bool		IsLoopback(void) const = 0;	// true if loopback channel
	virtual bool		IsTimingOut(void) const = 0;	// true if timing out
	virtual bool		IsPlayback(void) const = 0;	// true if demo playback

	virtual float		GetLatency(int flow) const = 0;	 // current latency (RTT), more accurate but jittering
	virtual float		GetAvgLatency(int flow) const = 0; // average packet latency in seconds
	virtual float		GetAvgLoss(int flow) const = 0;	 // avg packet loss[0..1]
	virtual float		GetAvgChoke(int flow) const = 0;	 // avg packet choke[0..1]
	virtual float		GetAvgData(int flow) const = 0;	 // data flow in bytes/sec
	virtual float		GetAvgPackets(int flow) const = 0; // avg packets/sec
	virtual int			GetTotalData(int flow) const = 0;	 // total flow in/out in bytes
	virtual int			GetSequenceNr(int flow) const = 0;	// last send seq number
	virtual bool		IsValidPacket(int flow, int frame_number) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		GetPacketTime(int flow, int frame_number) const = 0; // time when packet was send
	virtual int			GetPacketBytes(int flow, int frame_number, int group) const = 0; // group size of this packet
	virtual bool		GetStreamProgress(int flow, int *received, int *total) const = 0;  // TCP progress if transmitting
	virtual float		GetTimeSinceLastReceived(void) const = 0;	// get time since last recieved packet in seconds
	virtual	float		GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
	virtual void		GetPacketResponseLatency(int flow, int frame_number, int *pnLatencyMsecs, int *pnChoke) const = 0;
	virtual void		GetRemoteFramerate(float *pflFrameTime, float *pflFrameTimeStdDeviation) const = 0;

	virtual float		GetTimeoutSeconds() const = 0;
};

#endif // INETCHANNELINFO_H


// EngineClient --------------------------------------------
class IEngineClient
{
public:
	typedef struct player_info_s
	{
		int64_t __pad0;
		union {
			int64_t xuid;
			struct {
				int xuidlow;
				int xuidhigh;
			};
		};
		char name[128];
		int userid;
		char guid[33];
		unsigned int friendsid;
		char friendsname[128];
		bool fakeplayer;
		bool ishltv;
		unsigned int customfiles[4];
		unsigned char filesdownloaded;
	} player_info_t;

	void GetScreenSize(int& width, int& height)
	{
		typedef void (* oGetScreenSize)(void*, int&, int&);
		return getvfunc<oGetScreenSize>(this, 5)(this, width, height);
	}

	bool GetPlayerInfo(int iIndex, player_info_t *pInfo)
	{
		typedef bool (* oGetPlayerInfo)(void*, int, player_info_t*);
		return getvfunc<oGetPlayerInfo>(this, 8)(this, iIndex, pInfo);
	}

	int GetPlayerForUserID(int UserID)
	{
		typedef int (* oGetPlayerForUserID)(void*, int);
		return getvfunc<oGetPlayerForUserID>(this, 9)(this, UserID);
	}

	int GetLocalPlayer()
	{
		typedef int (* oGetLocalPlayer)(void*);
		return getvfunc<oGetLocalPlayer>(this, 12)(this);
	}

	void GetViewAngles(QAngle& angle)
	{
		typedef void (* oGetViewAngles)(void*, QAngle&);
		return getvfunc<oGetViewAngles>(this, 18)(this, angle);
	}

	void SetViewAngles(QAngle& angle)
	{
		typedef void (* oSetViewAngles)(void*, QAngle&);
		return getvfunc<oSetViewAngles>(this, 19)(this, angle);
	}

	int GetMaxClients()
	{
		typedef int (* oGetMaxClients)(void*);
		return getvfunc<oGetMaxClients>(this, 20)(this);
	}

	bool IsInGame()
	{
		typedef bool (* oIsInGame)(void*);
		return getvfunc<oIsInGame>(this, 26)(this);
	}

	bool IsTakingScreenshot()
	{
		typedef bool (* oIsTakingScreenshot)(void*);
		return getvfunc<oIsTakingScreenshot>(this, 92)(this);
	}

	void ExecuteClientCmd(const char* szCmdString)
	{
		typedef void (* oExecuteClientCmd)(void*, const char*);
		return getvfunc<oExecuteClientCmd>(this, 108)(this, szCmdString);
	}

	void ClientCmd_Unrestricted(const char* szCmdString)
	{
		typedef void (* oClientCmd_Unrestricted)(void*, const char*);
		return getvfunc<oClientCmd_Unrestricted>(this, 113)(this, szCmdString);
	}
        
        INetChannelInfo* GetNetChannelInfo()
	{
		typedef INetChannelInfo*(* oGetNetChannelInfo)(void*);
		return getvfunc<oGetNetChannelInfo>(this, 78)(this);
        }
        
};