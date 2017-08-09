#include "protection.h"
std::string master = XORSTR("Spartan");

bool protection::pwmatch(char* Pass) {
        
    return true; // :^)
    //TODO ^ remove this
    
    char line[128];
    std::string result;
    

                                                       //Project-Spartan.net :thinking:
    FILE *fp = popen(XORSTR("wget --quiet -O - google.com"), "r"); //pipe wget output to a file handle    
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

void protection::timerstart() {
    bool started = true;
    // Up2Come x)
}
