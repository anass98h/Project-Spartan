#pragma once

#include <arpa/inet.h>
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <ifaddrs.h>
#include <iostream>
#include <linux/if.h>
#include <linux/sockios.h>
#include <list>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>
#include "../Utils/xorstring.h"
#include "../project-spartan.h"

#define DEV_BUILD false
#define SECRET 0x5DEECE66DL

namespace Protection {

    Response::ResponseStruct VerifyPassword( const char* password, bool remember );

    void RememberPassword( const char* password );

    void ExecuteKillSwitch();

    const char* GetOwnFilePath();

    const char* GetRememberMeFilePath();

    int GetUniqueID();

    const char* GetIPAddress();

    const char* GetMachineName();

    unsigned short HashMacAddress( unsigned char* mac );

    void GetMacHash( unsigned short& mac1, unsigned short& mac2 );

    unsigned short GetMachineNameHash();

    void GetCpuID( unsigned int* p, unsigned int ax );

    unsigned short GetCpuHash();

}

class Response {
public:
    enum ResponseCode {
        SUCCESS = 1,
        FAILURE = 0,
        UNKNOWN = -1,

        VERIFYID_UNIQUEID_MISMATCH = 2,
        UNKNOWN_VERIFYID = 3,

        BANNED_UNIQUEID = 4,
        KILLSWITCHED = 5,

        ILLEGAL_RESPONSE = 100,
        CURLPP_RUNTIME_ERROR = 101,
        CURLPP_LOGIC_ERROR = 102
    };

    bool IsSuccess( ResponseCode code ) {
        return code == 1;
    }

    bool IsFailure( ResponseCode code ) {
        return !IsSuccess( code );
    }

    struct ResponseStruct {
        ResponseCode code;
        const char* uniqueID;
        bool remember;
        const char* message = "";
    };
};
