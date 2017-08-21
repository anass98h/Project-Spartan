#include "protection.h"
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

std::string master = XORSTR("Spartan");

const char* getMachineName() 
{ 
   static struct utsname u;  

   if ( uname( &u ) < 0 )    
   {       
      assert(0);             
      return "unknown";      
   }       

   return u.nodename;        
}   

unsigned short getNameHash()          
{ 
   
   unsigned char* sysname = (unsigned char*)getMachineName();       
   unsigned short hash = 0;             

   for ( unsigned int i = 0; sysname[i]; i++ )         
      hash += ( sysname[i] << (( i & 1 ) * 8 ));       

   return hash;              
} 

#ifdef DARWIN                
 #include <mach-o/arch.h>    
 unsigned short getCpuHash()            
 {         
     const NXArchInfo* info = NXGetLocalArchInfo();    
     unsigned short val = 0;            
     val += (unsigned short)info->cputype;               
     val += (unsigned short)info->cpusubtype;            
     return val;             
 }         

#else // !DARWIN             

 static void getCpuid( unsigned int* p, unsigned int ax )       
 {         
    __asm __volatile         
    (   "movl %%ebx, %%esi\n\t"               
        "cpuid\n\t"          
        "xchgl %%ebx, %%esi" 
        : "=a" (p[0]), "=S" (p[1]),           
          "=c" (p[2]), "=d" (p[3])            
        : "0" (ax)           
    );     
 }         

 unsigned short getCpuHash()            
 {         
    unsigned int cpuinfo[4] = { 0, 0, 0, 0 };          
    getCpuid( cpuinfo, 0 );  
    unsigned short hash = 0;            
    unsigned int* ptr = (&cpuinfo[0]);                 
    for ( unsigned int i = 0; i < 4; i++ )             
       hash += (ptr[i] & 0xFFFF) + ( ptr[i] >> 16 );   

    return hash;             
 }         
#endif // !DARWIN            
 
bool protection::pwmatch(char* Pass) {
        
    return true; // :^)
    //TODO ^ remove this
    
    char line[128];
    std::string result;
    

                                                       //Project-Spartan.net :thinking:
    FILE *fp = popen(XORSTR("wget --quiet -O - whatKills.us/psecuritymemez"), "r"); //pipe wget output to a file handle    
    while (fgets(line, 128, fp))
        result += line;

    pclose(fp); //close pipe

    std::string str1(Pass);
    if (str1 == master && str1.length() == master.length()) {
        return true;
    }
    else {
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

void protection::UniqueId () {
    int cH = getCpuHash();
    int nH = getNameHash();
   std::string n = getMachineName();
   int uniqueId = (((cH+nH)*n.length())/3.20438573);
}

void protection::timerstart() {
    bool started = true;
    // Up2Come x)
}
