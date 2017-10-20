#include "hooks.h"

void Hooks::OverrideConfig( void *thisptr, MaterialSystem_Config_t *config, bool forceUpdate ) {
    MaterialConfig::OverrideConfig( config, forceUpdate );

    materialVMT->GetOriginalMethod<OverrideConfigFn>( 21 )( thisptr, config, forceUpdate );
}