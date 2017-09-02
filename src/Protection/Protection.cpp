#include "Protection.h"

const char* Protection::lastSha256UniqueID = Util::GetSHA256(XORSTR("null"));
bool Protection::lastAllowRemember = false;
const char* Protection::lastMessage = XORSTR("You shouldn't see this default message.");

// Right now we're using ImGui for verification
// This is temporary and I'll look into a Chods-Cheats style
// verification system where we have a external program that
// checks for UniqueID and LoginID and then injects.

ResponseStatus Protection::VerifyPassword(const char* password)
{
    if(IS_DEVELOPMENT_PREVIEW)
        password = XORSTR("DEVELOPER_PREVIEW_QUICKBUILD");

    unsigned short mac1 = 0;
    unsigned short mac2 = 0;
    Protection::GetMacHash(mac1, mac2);

    /*
     * Example request sent from client side:
     * {
     *    "uniqueID": "protection unique id",
     *    "verifyID": "verify id the user input",
     *    "timestamp": "2011-10-08T07:07:09Z",
     *    "trace": {
     *      "machineName": "marc-arch",
     *      "machineNameHash": "hash of the machine name",
     *      "cpuHash": "hash of the cpu id",
     *      "mac": {
     *          "one": "first mac address"
     *          "two": "second mac address"
     *      },
     *      "ipAddress": "127.0.0.1",
     *      "dev": false
     *    }
     * }
     */

    Json::Value requestJson;

    requestJson[XORSTR("uniqueID")] = Protection::GetUniqueID();
    requestJson[XORSTR("verifyID")] = password;
    requestJson[XORSTR("timestamp")] = Util::GetCurrentTimeAsISO8601Timestamp();

    // These are "trace" debug arguments and should be committed out as the web server doesn't accept real requests with these arguments
    requestJson[XORSTR("trace")][XORSTR("machineName")] = Protection::GetMachineName();
    requestJson[XORSTR("trace")][XORSTR("machineNameHash")] = Protection::GetMachineNameHash();
    requestJson[XORSTR("trace")][XORSTR("cpuHash")] = Protection::GetCpuHash();
    requestJson[XORSTR("trace")][XORSTR("mac")][XORSTR("one")] = mac1;
    requestJson[XORSTR("trace")][XORSTR("mac")][XORSTR("two")] = mac2;
    requestJson[XORSTR("trace")][XORSTR("ipAddress")] = Protection::GetIPAddress();
    requestJson[XORSTR("trace")][XORSTR("dev")] = IS_DEVELOPMENT_PREVIEW;

    Json::FastWriter writer;
    std::string body = writer.write(requestJson);

    try
    {
        curlpp::Cleanup cleanup;

        std::string uniqueIDHash = std::string(XORSTR("X-Spartan-SHA256: "));
        uniqueIDHash.append(Util::GetSHA256(std::to_string(Protection::GetUniqueID()).c_str()));

        std::list<std::string> headers =
        {
            XORSTR("Content-Type: application/json; charset=utf-8"),
            uniqueIDHash
        };

        curlpp::Easy request;
        request.setOpt(new curlpp::options::Url(XORSTR("https://spartan.whatkills.us:77/verify/")));
        request.setOpt(new curlpp::options::HttpHeader(headers));
        request.setOpt(new curlpp::options::UserAgent(XORSTR("Project-Spartan Beta")));
        request.setOpt(new curlpp::options::PostFields(body));
        request.setOpt(new curlpp::options::PostFieldSize(body.length()));

        std::ostringstream response;
        request.setOpt(new curlpp::options::WriteStream(&response));

        request.perform(); // TODO: Perform request async

        Json::Value value;
        Json::Reader reader;

        if (!reader.parse(response.str().c_str(), value))
            return ResponseStatus::ILLEGAL_RESPONSE;

        /*
         * Example response from server side:
         * {
         *    "mode": "success",
         *    "uniqueid": "sha256 of uniqueid",
         *    "remember": true,
         *    "message": "optional message when failed"
         * }
         */

        Protection::lastSha256UniqueID = value.get(XORSTR("uniqueid"), XORSTR("0")).asCString();
        Protection::lastAllowRemember = value.get(XORSTR("remember"), false).asBool();
        Protection::lastMessage = value.get(XORSTR("message"), XORSTR("An error occured.")).asCString();

        return ProtectionUtil::ParseStringToStatus(value.get(XORSTR("mode"), XORSTR("FAILURE")).asCString());
    }
    catch (curlpp::RuntimeError &ex)
    {
        cvar->ConsoleColorPrintf(ColorRGBA(255, 0, 0), XORSTR("A runtime error occured when trying to contact Spartan verification server."));

        return ResponseStatus::CURLPP_RUNTIME_ERROR;
    }
    catch (curlpp::LogicError &ex)
    {
        cvar->ConsoleColorPrintf(ColorRGBA(255, 0, 0), XORSTR("A logic error occured when trying to contact Spartan verification server."));

        return ResponseStatus::CURLPP_LOGIC_ERROR;
    }

    return ResponseStatus::UNKNOWN;
}

int Protection::GetUniqueID()
{
    std::string machineName = Protection::GetMachineName();

    // If we don't find MAC hashes we just add twice 0, which is not a problem
    unsigned short mac1 = 0;
    unsigned short mac2 = 0;
    Protection::GetMacHash(mac1, mac2);

    int uniqueID = static_cast<int>(
        ((Protection::GetCpuHash() + Protection::GetMachineNameHash() + mac1 + mac2) * machineName.length()) ^ SECRET
    );

    return uniqueID;
}

// Ghetto way
const char* Protection::GetOwnFilePath()
{
    FILE* selfMaps = fopen(XORSTR("/proc/self/maps"), XORSTR("r"));
    if(selfMaps == NULL)
        return XORSTR("INVALID");

    const char* filePath = XORSTR("INVALID");

    char line[PATH_MAX];
    while (fgets(line, PATH_MAX, selfMaps))
    {
        std::string curLine(line);

        //cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "File Path: %s\n", line);

        if(curLine.find(XORSTR("Spartan.so")) != std::string::npos)
        {
            cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), XORSTR("File Path: %s\n"), line);

            size_t pathPos = curLine.find(XORSTR("/home/"));
            if (pathPos != std::string::npos)
            {
                cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), XORSTR("Needle in the hay found at: %i\n"), pathPos);

                filePath = curLine.substr(pathPos).c_str();

                break;
            }
        }
    }

    fclose(selfMaps);

    cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), XORSTR("Spartan.so File Path: %s\n"), filePath);

    return filePath;
}

const char* Protection::GetIPAddress()
{
    const char* ipAddress = XORSTR("INVALID");

    struct ifaddrs * ifAddrStruct = NULL;
    struct ifaddrs * ifa = NULL;
    void * tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
            continue;

        // Maybe implement IPv6 support later when its actually used
        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            tmpAddrPtr = &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;
            char adressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, adressBuffer, INET_ADDRSTRLEN);

            ipAddress = adressBuffer;
        }
    }

    if (ifAddrStruct != NULL)
        freeifaddrs(ifAddrStruct);

    return ipAddress;
}

const char* Protection::GetMachineName()
{
    static struct utsname u;

    if (uname(&u) < 0)
    {
        assert(0);
        return XORSTR("unknown");
    }

    return u.nodename;
}

unsigned short Protection::HashMacAddress(unsigned char* mac)
{
    unsigned short hash = 0;

    for ( unsigned int i = 0; i < 6; i++ )
    {
        hash += ( mac[i] << (( i & 1 ) * 8 ));
    }
    return hash;
}

void Protection::GetMacHash(unsigned short& mac1, unsigned short& mac2)
{
    mac1 = 0;
    mac2 = 0;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) return;

    // enumerate all IP addresses of the system
    struct ifconf conf;
    char ifconfbuf[128 * sizeof(struct ifreq)];
    memset(ifconfbuf, 0, sizeof(ifconfbuf));
    conf.ifc_buf = ifconfbuf;
    conf.ifc_len = sizeof(ifconfbuf);
    if (ioctl(sock, SIOCGIFCONF, &conf))
    {
        assert(0);
        return;
    }

    // get MAC address
    bool foundMac1 = false;
    struct ifreq *ifr;
    for (ifr = conf.ifc_req; (char *) ifr < (char *) conf.ifc_req + conf.ifc_len; ifr++)
    {
        if (ifr->ifr_addr.sa_data == (ifr + 1)->ifr_addr.sa_data)
            continue;  // duplicate, skip it

        if (ioctl(sock, SIOCGIFFLAGS, ifr))
            continue;  // failed to get flags, skip it
        if (ioctl(sock, SIOCGIFHWADDR, ifr) == 0)
        {
            if (!foundMac1)
            {
                foundMac1 = true;
                mac1 = Protection::HashMacAddress((unsigned char *) &(ifr->ifr_addr.sa_data));
            }
            else
            {
                mac2 = Protection::HashMacAddress((unsigned char *) &(ifr->ifr_addr.sa_data));
                break;
            }
        }
    }

    close(sock);

    // sort the mac addresses. We don't want to invalidate
    // both macs if they just change order.
    if (mac1 > mac2)
    {
        unsigned short tmp = mac2;
        mac2 = mac1;
        mac1 = tmp;
    }
}

unsigned short Protection::GetMachineNameHash()
{
    unsigned char* sysname = (unsigned char*) Protection::GetMachineName();
    unsigned short hash = 0;

    for (unsigned int i = 0; sysname[i]; i++)
        hash += (sysname[i] << ((i & 1) * 8));

    return hash;
}

static void Protection::GetCpuID(unsigned int* p, unsigned int ax)
{
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

unsigned short Protection::GetCpuHash()
{
    unsigned int cpuinfo[4] = { 0, 0, 0, 0 };
    Protection::GetCpuID(cpuinfo, 0);
    unsigned short hash = 0;
    unsigned int* ptr = (&cpuinfo[0]);
    for (unsigned int i = 0; i < 4; i++)
        hash += (ptr[i] & 0xFFFF) + (ptr[i] >> 16);

    return hash;
}
