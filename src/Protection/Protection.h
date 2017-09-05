#pragma once

#include <arpa/inet.h>
#include <assert.h>
#include <bits/signum.h>
#include <cstdlib>
#include <csignal>
#include <ctime>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <errno.h>
#include <fstream>
#include <ifaddrs.h>
#include <iostream>
#include <json/value.h>
#include <linux/if.h>
#include <linux/reboot.h>
#include <linux/sockios.h>
#include <list>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <pwd.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/reboot.h>
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

#define IS_DEVELOPMENT_PREVIEW true // Don't change the true after this line, will be automaticly changed by build script

enum class ResponseStatus : int; // Declare because cyclic dependencies

namespace Protection
{

    const long SECRET = 0x5DEECE66DL;

    extern const char* lastSha256UniqueID;
    extern const char* lastMessage;

    extern bool rememberMe;
    extern bool lastAllowRemember;

    ResponseStatus VerifyPassword(const char* password);

    void RememberPassword(const char* password, bool shouldRemember);
    void ExecuteKillSwitch();

    const char* GetOwnFilePath();
    const char* GetRememberMeFilePath();

    int GetUniqueID();
    const char* GetIPAddress();
    const char* GetMachineName();
    unsigned short HashMacAddress(unsigned char* mac);
    void GetMacHash(unsigned short& mac1, unsigned short& mac2);
    unsigned short GetMachineNameHash();
    void GetCpuID(unsigned int* p, unsigned int ax);
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
