#include "protection.h"
std::string master = "Spartan";

	bool protection::pwmatch (char* Pass)
	{

            char line[128];
            std::string result;


			
			FILE *fp=popen("wget --quiet -O - chrey.xyz/nqr","r"); //pipe wget output to a file handle    
              while (fgets(line, 128, fp))
                result += line;

            pclose(fp); //close pipe
   
		std::string str1(Pass);
           if(str1 == master && str1.length() == master.length() ){ return true;} 
		else{
		if(str1.length() ==16 ){
				  std::size_t found = result.find(str1); 
			if (found!=std::string::npos )
			{            
            	return true;                    
			
                                }  
                                   
                            
			
			else{
			
			return false;
								}	
									}
	else{ return false;}

	
		}

	}
 void protection::timerstart()
{
   bool started = true;
   // Up2Come x)
}
