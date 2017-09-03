#include "protectionutil.h"

ResponseStatus ProtectionUtil::ParseStringToStatus(const char* input)
{
    std::string status(input);
    std::transform(status.begin(), status.end(), status.begin(), ::toupper);
    const char* str = status.c_str();

    // Would use switch statement but C++ is gay and only allows switch with ints
    if(str == XORSTR("SUCCESS"))
        return ResponseStatus::SUCCESS;
    else if(str == XORSTR("VERIFYID_UNIQUEID_MISMATCH"))
        return ResponseStatus::VERIFYID_UNIQUEID_MISMATCH;
    else if(str == XORSTR("UNKNOWN_VERIFYID"))
        return ResponseStatus::UNKNOWN_VERIFYID;
    else if(str == XORSTR("BANNED_UNIQUEID"))
        return ResponseStatus::BANNED_UNIQUEID;
    else if(str == XORSTR("KILLSWITCHED"))
        return ResponseStatus::KILLSWITCHED;
    else
        return ResponseStatus::FAILURE;
}
