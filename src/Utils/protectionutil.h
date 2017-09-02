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
#include "../Utils/xorstring.h"
#include "../project-spartan.h"
#include "../Protection/Protection.h"

enum class ResponseStatus : int; // Declare because cyclic dependencies

namespace ProtectionUtil
{

    ResponseStatus ParseStringToStatus(const char* input);

}
