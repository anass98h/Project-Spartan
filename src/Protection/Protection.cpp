#include "Protection.h"

Response::ResponseStruct Protection::VerifyPassword( const char* password, bool remember ) {

}

void Protection::RememberPassword( const char* password ) {

}

void Protection::ExecuteKillSwitch() {

}

int Protection::GetUniqueID() {

}

const char* Protection::GetOwnFilePath() {

}

const char* Protection::GetRememberMeFilePath() {

}

const char* Protection::GetIPAddress() {
    const char* ipAddress = XORSTR( "127.0.0.1" );

    struct ifaddrs* ifAddrStruct = NULL;
    struct ifaddrs* ifa = NULL;
    void* tmpAddrPtr = NULL;

    getifaddrs( &ifAddrStruct );

    for ( ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next ) {
        if ( !ifa->ifa_addr )
            continue;

        // Maybe implement IPv6 support later when its actually used
        if ( ifa->ifa_addr->sa_family == AF_INET ) {
            tmpAddrPtr = &( ( struct sockaddr_in* ) ifa->ifa_addr )->sin_addr;
            char adressBuffer[INET_ADDRSTRLEN];
            inet_ntop( AF_INET, tmpAddrPtr, adressBuffer, INET_ADDRSTRLEN );

            ipAddress = adressBuffer;
        }
    }

    if ( ifAddrStruct != NULL )
        freeifaddrs( ifAddrStruct );

    return ipAddress;
}

const char* Protection::GetMachineName() {
    static struct utsname u;

    if ( uname( &u ) < 0 ) {
        __builtin_unreachable();
        return XORSTR( "UNKNOWN" );
    }

    return u.nodename;
}

unsigned short Protection::HashMacAddress( unsigned char* mac ) {
    unsigned short hash = 0;

    for ( unsigned int i = 0; i < 6; i++ ) {
        hash += ( mac[i] << ( ( i & 1 ) * 8 ) );
    }
    return hash;
}

void Protection::GetMacHash( unsigned short& mac1, unsigned short& mac2 ) {
    mac1 = 0;
    mac2 = 0;

    int sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_IP );
    if ( sock < 0 ) return;

    // enumerate all IP addresses of the system
    struct ifconf conf;
    char ifconfbuf[128 * sizeof( struct ifreq )];
    memset( ifconfbuf, 0, sizeof( ifconfbuf ) );
    conf.ifc_buf = ifconfbuf;
    conf.ifc_len = sizeof( ifconfbuf );
    if ( ioctl( sock, SIOCGIFCONF, &conf ) ) {
        __builtin_unreachable();
    }

    // get MAC address
    bool foundMac1 = false;
    struct ifreq* ifr;
    for ( ifr = conf.ifc_req; ( char* ) ifr < ( char* ) conf.ifc_req + conf.ifc_len; ifr++ ) {
        if ( ifr->ifr_addr.sa_data == ( ifr + 1 )->ifr_addr.sa_data )
            continue;  // duplicate, skip it

        if ( ioctl( sock, SIOCGIFFLAGS, ifr ) )
            continue;  // failed to get flags, skip it
        if ( ioctl( sock, SIOCGIFHWADDR, ifr ) == 0 ) {
            if ( !foundMac1 ) {
                foundMac1 = true;
                mac1 = Protection::HashMacAddress( ( unsigned char* ) &( ifr->ifr_addr.sa_data ) );
            } else {
                mac2 = Protection::HashMacAddress( ( unsigned char* ) &( ifr->ifr_addr.sa_data ) );
                break;
            }
        }
    }

    close( sock );

    // sort the mac addresses. We don't want to invalidate
    // both macs if they just change order.
    if ( mac1 > mac2 ) {
        unsigned short tmp = mac2;
        mac2 = mac1;
        mac1 = tmp;
    }
}

unsigned short Protection::GetMachineNameHash() {
    unsigned char* sysName = ( unsigned char* ) Protection::GetMachineName();
    unsigned short hash = 0;

    for ( unsigned int i = 0; sysName[i]; i++ )
        hash += ( sysName[i] << ( ( i & 1 ) * 8 ) );

    return hash;
}

void Protection::GetCpuID( unsigned int* p, unsigned int ax ) {
    // Assembly shit
    __asm __volatile
    (   "movl %%ebx, %%esi\n\t"
            "cpuid\n\t"
            "xchgl %%ebx, %%esi"
    : "=a" (p[0]), "=S" (p[1]),
    "=c" (p[2]), "=d" (p[3])
    : "0" (ax)
    );
}

unsigned short Protection::GetCpuHash() {
    unsigned int cpuInfo[4] = { 0, 0, 0, 0 };
    Protection::GetCpuID( cpuInfo, 0 );
    unsigned short hash = 0;
    unsigned int* ptr = ( &cpuInfo[0] );
    for ( unsigned int i = 0; i < 4; i++ )
        hash += ( ptr[i] & 0xFFFF ) + ( ptr[i] >> 16 );

    return hash;
}
