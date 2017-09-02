#pragma once

#include <arpa/inet.h>
#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <errno.h>
#include <ifaddrs.h>
#include <iostream>
#include <json/value.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <list>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>
#include "../Utils/picosha2.h"
#include "../Utils/protectionutil.h"
#include "../Utils/xorstring.h"
#include "../project-spartan.h"

#define IS_DEVELOPMENT_PREVIEW true // Keep this on line 40, line will be set according to build script

enum class ResponseStatus : int; // Declare because cyclic dependencies

namespace Protection
{

    const long SECRET = 0x5DEECE66DL;

    extern const char* lastSha256UniqueID;
    extern bool lastAllowRemember;
    extern const char* lastMessage;

    ResponseStatus VerifyPassword(const char* password);
    void ExecuteKillSwitch();

    const char* GetOwnFilePath();

    int GetUniqueID();
    const char* GetIPAddress();
    const char* GetMachineName();
    unsigned short HashMacAddress(unsigned char* mac);
    void GetMacHash(unsigned short& mac1, unsigned short& mac2);
    unsigned short GetMachineNameHash();
    static void GetCpuID(unsigned int* p, unsigned int ax);
    unsigned short GetCpuHash();

}

enum class ResponseStatus : int
{

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
