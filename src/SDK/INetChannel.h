#pragma once

#if !defined (INETCHANNEL_H)
#define INETCHANNEL_H

#include "SDK.h"


//class	IDemoRecorder;
class INetMessage;

class INetChannelHandler;

class INetChannelInfo;
//typedef struct netpacket_s netpacket_t;
//typedef struct netadr_s	netadr_t;

class INetChannel : public INetChannelInfo {
public:
    virtual         ~INetChannel( void ) {};

    virtual void SetDataRate( float rate ) = 0;

    virtual bool RegisterMessage( INetMessage* msg ) = 0;

    virtual bool StartStreaming( unsigned int challengeNr ) = 0;

    virtual void ResetStreaming( void ) = 0;

    virtual void SetTimeout( float seconds ) = 0;

//	virtual void	SetDemoRecorder(IDemoRecorder *recorder) = 0;
    virtual void SetChallengeNr( unsigned int chnr ) = 0;

    virtual void Reset( void ) = 0;

    virtual void Clear( void ) = 0;

    virtual void Shutdown( const char* reason ) = 0;

    virtual void ProcessPlayback( void ) = 0;

    virtual bool ProcessStream( void ) = 0;
//	virtual void	ProcessPacket( struct netpacket_s* packet, bool bHasHeader ) = 0;

    virtual bool SendNetMsg( INetMessage& msg, bool bForceReliable = false, bool bVoice = false ) = 0;

#ifdef POSIX
    FORCEINLINE bool SendNetMsg(INetMessage const &msg, bool bForceReliable = false, bool bVoice = false ) { return SendNetMsg( *( (INetMessage *) &msg ), bForceReliable, bVoice ); }
#endif

    //virtual bool	SendData(bf_write &msg, bool bReliable = true) = 0;
    virtual bool SendFile( const char* filename, unsigned int transferID ) = 0;

    virtual void DenyFile( const char* filename, unsigned int transferID ) = 0;

    virtual void RequestFile_OLD( const char* filename,
                                  unsigned int transferID ) = 0;    // get rid of this function when we version the
    virtual void SetChoked( void ) = 0;

    //virtual int		SendDatagram(bf_write *data) = 0;
    virtual bool Transmit( bool onlyReliable = false ) = 0;

    //virtual const netadr_t	&GetRemoteAddress( void ) const = 0;
    virtual INetChannelHandler* GetMsgHandler( void ) const = 0;

    virtual int GetDropNumber( void ) const = 0;

    virtual int GetSocket( void ) const = 0;

    virtual unsigned int GetChallengeNr( void ) const = 0;

    virtual void GetSequenceData( int& nOutSequenceNr, int& nInSequenceNr, int& nOutSequenceNrAck ) = 0;

    virtual void SetSequenceData( int nOutSequenceNr, int nInSequenceNr, int nOutSequenceNrAck ) = 0;

    virtual void UpdateMessageStats( int msggroup, int bits ) = 0;

    virtual bool CanPacket( void ) const = 0;

    virtual bool IsOverflowed( void ) const = 0;

    virtual bool IsTimedOut( void ) const = 0;

    virtual bool HasPendingReliableData( void ) = 0;

    virtual void SetFileTransmissionMode( bool bBackgroundMode ) = 0;

    virtual void SetCompressionMode( bool bUseCompression ) = 0;

    virtual unsigned int RequestFile( const char* filename ) = 0;

    virtual float GetTimeSinceLastReceived( void ) const = 0;    // get time since last received packet in seconds

    virtual void SetMaxBufferSize( bool bReliable, int nBytes, bool bVoice = false ) = 0;

    virtual bool IsNull() const = 0;

    virtual int GetNumBitsWritten( bool bReliable ) = 0;

    virtual void SetInterpolationAmount( float flInterpolationAmount ) = 0;

    virtual void SetRemoteFramerate( float flFrameTime, float flFrameTimeStdDeviation ) = 0;

    // Max # of payload bytes before we must split/fragment the packet
    virtual void SetMaxRoutablePayloadSize( int nSplitSize ) = 0;

    virtual int GetMaxRoutablePayloadSize() = 0;

    virtual int GetProtocolVersion() = 0;
};


#endif // INETCHANNEL_H

#if !defined( INETMSGHANDLER_H )
#define INETMSGHANDLER_H


class INetChannel;
//typedef struct netpacket_s netpacket_t;

class INetChannelHandler {
public:
    virtual    ~INetChannelHandler( void ) {};

    virtual void ConnectionStart( INetChannel* chan ) = 0;    // called first time network channel is established

    virtual void ConnectionClosing( const char* reason ) = 0; // network channel is being closed by remote site

    virtual void ConnectionCrashed( const char* reason ) = 0; // network error occured

    virtual void
    PacketStart( int incoming_sequence, int outgoing_acknowledged ) = 0;    // called each time a new packet arrived

    virtual void PacketEnd( void ) = 0; // all messages has been parsed

    virtual void
    FileRequested( const char* fileName, unsigned int transferID ) = 0; // other side request a file for download

    virtual void FileReceived( const char* fileName, unsigned int transferID ) = 0; // we received a file

    virtual void
    FileDenied( const char* fileName, unsigned int transferID ) = 0;    // a file request was denied by other side

    virtual void FileSent( const char* fileName, unsigned int transferID ) = 0;    // we sent a file
};

#define PROCESS_NET_MESSAGE( name )    \
    virtual bool Process##name( NET_##name *msg )

#define PROCESS_SVC_MESSAGE( name )    \
    virtual bool Process##name( SVC_##name *msg )

#define PROCESS_CLC_MESSAGE( name )    \
    virtual bool Process##name( CLC_##name *msg )

#define PROCESS_MM_MESSAGE( name )    \
    virtual bool Process##name( MM_##name *msg )


#define REGISTER_NET_MSG( name )                \
    NET_##name * p##name = new NET_##name();    \
    p##name->m_pMessageHandler = this;            \
    chan->RegisterMessage( p##name );            \

#define REGISTER_SVC_MSG( name )                \
    SVC_##name * p##name = new SVC_##name();    \
    p##name->m_pMessageHandler = this;            \
    chan->RegisterMessage( p##name );            \

#define REGISTER_CLC_MSG( name )                \
    CLC_##name * p##name = new CLC_##name();    \
    p##name->m_pMessageHandler = this;            \
    chan->RegisterMessage( p##name );            \

#define REGISTER_MM_MSG( name )                    \
    MM_##name * p##name = new MM_##name();        \
    p##name->m_pMessageHandler = this;            \
    chan->RegisterMessage( p##name );            \



#endif // INETMSGHANDLER_H


#ifndef INETMESSAGE_H
#define INETMESSAGE_H


class INetMessage;

class INetChannel;

// typedef bool (INetMsgHandler::*PROCESSFUNCPTR)(INetMessage*);
// #define CASTPROCPTR( fn ) static_cast <bool (INetMsgHandler::*)(INetMessage*)> (fn)

class INetMessage {
public:
    virtual    ~INetMessage() {};

    // Use these to setup who can hear whose voice.
    // Pass in client indices (which are their ent indices - 1).

    virtual void SetNetChannel( INetChannel* netchan ) = 0; // netchannel this message is from/for
    virtual void SetReliable( bool state ) = 0;    // set to true if it's a reliable message

    virtual bool Process( void ) = 0; // calles the recently set handler to process this message



    virtual bool IsReliable( void ) const = 0;  // true, if message needs reliable handling

    virtual int GetType( void ) const = 0; // returns module specific header tag eg svc_serverinfo
    virtual int GetGroup( void ) const = 0;    // returns net message group of this message
    virtual const char* GetName( void ) const = 0;    // returns network message name, eg "svc_serverinfo"
    virtual INetChannel* GetNetChannel( void ) const = 0;

    virtual const char* ToString( void ) const = 0; // returns a human readable string about message content
};


#endif

#ifndef INETWORK_H
#define INETWORK_H


class IConnectionlessPacketHandler;

class INetwork {
public:
    virtual    ~INetwork( void ) {};

    virtual void Init( void ) = 0;

    virtual void Config( bool multiplayer );

    virtual void IsMultiplayer( void ) = 0; // true = full MP mode, false = loopback SP mode
    virtual void IsEnabled( void ) = 0;

    // shuts down Network, closes all UPD/TCP channels
    virtual void Shutdown( void ) = 0;

    // must be called each system frame to do any asynchronouse TCP stuff
    virtual void RunFrame( double time ) = 0;


    virtual void LogBadPacket( netpacket_t* packet ) = 0;

    // Address conversion
//	virtual bool StringToAdr ( const char *s, netadr_t *a) = 0;

    // Convert from host to network byte ordering
    virtual unsigned short HostToNetShort( unsigned short us_in );

    // and vice versa
    virtual unsigned short NetToHostShort( unsigned short us_in );


};


#endif // INETWORK_H

