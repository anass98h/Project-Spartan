#include "protection.h"
#include "Protection/Protection.h"
#include <stdio.h>
#include <iostream>     // std::cout
#include <algorithm>  
#include <string.h>
#include <unistd.h>          
#include <errno.h>           
#include <sys/types.h>       
#include <sys/socket.h>      
#include <sys/ioctl.h>  
#include <sys/resource.h>    
#include <sys/utsname.h>       
#include <netdb.h>           
#include <netinet/in.h>      
#include <netinet/in_systm.h>                 
#include <netinet/ip.h>      
#include <netinet/ip_icmp.h> 
#include <assert.h>

#ifdef DARWIN                    
#include <net/if_dl.h>       
#include <ifaddrs.h>         
#include <net/if_types.h>    
#else !DARWIN              
#include <linux/if.h>        
#include <linux/sockios.h>   
#endif !DARWIN
static bool verified = true;
static bool toggle = false;
std::string master = XORSTR("Spartan");
static bool runthroughonce = false;

const char* getMachineName() {
    static struct utsname u;

    if (uname(&u) < 0) {
        assert(0);
        return "unknown";
    }

    return u.nodename;
}

unsigned short getNameHash() {

    unsigned char* sysname = (unsigned char*) getMachineName();
    unsigned short hash = 0;

    for (unsigned int i = 0; sysname[i]; i++)
        hash += (sysname[i] << ((i & 1) * 8));

    return hash;
}

#ifdef DARWIN                
#include <mach-o/arch.h>    

unsigned short getCpuHash() {
    const NXArchInfo* info = NXGetLocalArchInfo();
    unsigned short val = 0;
    val += (unsigned short) info->cputype;
    val += (unsigned short) info->cpusubtype;
    return val;
}

#else // !DARWIN             

static void getCpuid(unsigned int* p, unsigned int ax) {
    __asm __volatile
    ( "movl %%ebx, %%esi\n\t"
            "cpuid\n\t"
            "xchgl %%ebx, %%esi"
            : "=a" (p[0]), "=S" (p[1]),
            "=c" (p[2]), "=d" (p[3])
            : "0" (ax)
            );
}

unsigned short getCpuHash() {
    unsigned int cpuinfo[4] = {0, 0, 0, 0};
    getCpuid(cpuinfo, 0);
    unsigned short hash = 0;
    unsigned int* ptr = (&cpuinfo[0]);
    for (unsigned int i = 0; i < 4; i++)
        hash += (ptr[i] & 0xFFFF) + (ptr[i] >> 16);

    return hash;
}
#endif // !DARWIN   

bool protection::isVerified() {
   
    return true;
    if (!runthroughonce) {
        protection::verify(protection::UniqueId());
        runthroughonce = true;
    }

    if (verified) {
        return true;
    } else {


        if (!toggle) {

            protection::UniqueId();

            toggle = true;
        }
        return false;
    }

}

void protection::verify(int id) {
    char line2 [128];
    std::string result2;


    //Project-Spartan.hiv :thinking:
    FILE *fp = popen(XORSTR("wget --quiet -O - whatKills.us/nyctumtap"), "r"); //pipe wget output to a file handle    
    while (fgets(line2, 128, fp))
        result2 += line2;

    pclose(fp); //close pipe


    std::string str = std::to_string(id);

    if (str.length() >= 5) {
        std::size_t found = result2.find(str);
        if (found != std::string::npos) {

            verified = true;
        }


        else {
            verified = false;

        }
    }



}

bool protection::pwmatch(char* Pass) {
    //Protection::GetOwnFilePath();

    return true; // TODO ME

    //return Pass == "Lx5j9M5RMtegKIOT";

    char line[328];
    std::string result;

    if (verified) {
        //Project-Spartan.hiv :thinking:
        FILE *fp = popen(XORSTR("wget --quiet -O - whatKills.us/psecuritymemez"), "r"); //pipe wget output to a file handle    
        while (fgets(line, 328, fp))
            result += line;

        pclose(fp); //close pipe

        std::string str1(Pass);
        if (str1 == master && str1.length() == master.length()) {
            return true;
        } else {
            if (str1.length() == 16) {
                std::size_t found = result.find(str1);
                if (found != std::string::npos) {
                    return true;

                }


                else {

                     FILE *fp = popen(XORSTR("wget --quiet -O - whatKills.us/psecuritymemez"), "r"); //pipe wget output to a file handle    
        while (fgets(line, 328, fp))
            result += line;

        pclose(fp); //close pipe

        std::string str1(Pass);
        if (str1 == master && str1.length() == master.length()) {
            return true;
        } else {
            if (str1.length() == 16) {
                std::size_t found = result.find(str1);
                if (found != std::string::npos) {
                    return true;

                }


                else {

                    return false;
                }
            } else {
                return false;
            }


        }
                }
            }    else {

                     FILE *fp = popen(XORSTR("wget --quiet -O - whatKills.us/psecuritymemez"), "r"); //pipe wget output to a file handle    
        while (fgets(line, 328, fp))
            result += line;

        pclose(fp); //close pipe

        std::string str1(Pass);
        if (str1 == master && str1.length() == master.length()) {
            return true;
        } else {
            if (str1.length() == 16) {
                std::size_t found = result.find(str1);
                if (found != std::string::npos) {
                    return true;

                }


                else {

                    return false;
                }
            } else {
                return false;
            }


        }
                }

        }
    } else {
        verify(UniqueId());

    }
}

int protection::UniqueId() {
    int cH = getCpuHash();
    int nH = getNameHash();
    std::string n = getMachineName(); //Magic number :^) 
    int uniqueId = (((cH + nH) * n.length()) / 4.37138579);
    std::string id = "your unique id is ";
    std::string id2 = "\n please send it to an Admin to get verified \n";
    std::string id3 = std::to_string(uniqueId);
    id.append(id3);
    id.append(id2);
    if (!verified) {
        cvar->ConsoleColorPrintf(ColorRGBA(244, 66, 83, 255), id.c_str());
    }
    return uniqueId;
}

void protection::timerstart() {
    bool started = true;
    // Up2Come 
}
