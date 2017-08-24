#!/bin/sh

prefix="\e[34m\e[1mSpartan >\e[0m"
error_prefix="\e[91m\e[1mSpartan >\e[0m"
success_prefix="\e[32m\e[1mSpartan >\e[0m"

# ----------------------------------------------- #
#                                                 #
#    (                                            #
#    )\ )                      )                  #
#    (()/(           )  (    ( /(    )            #
#    /(_))`  )   ( /(  )(   )\())( /(   (         #
#    (_))  /(/(   )(_))(()\ (_))/ )(_))  )\ )     #
#    / __|((_)_\ ((_)_  ((_)| |_ ((_)_  _(_/(     #
#    \__ \| '_ \)/ _` || '_||  _|/ _` || ' \))    #
#    |___/| .__/ \__,_||_|   \__|\__,_||_||_|     #
#         |_|                                     #
#                                                 #
# ----------------------------------------------- #

csgo=$(pidof csgo_linux64)

if [ -z "$csgo" ]; then
    echo -e "$error_prefix Please open Counter-Strike: Global Offensive and inject."
    exit -1
fi

if ! grep -q "Spartan.so" /proc/$csgo/maps; then
    echo -e "$error_prefix Project Spartan needs to be injected before unloading."
    exit -1
fi

sudo gdb -n -q -batch-silent \
    -ex "set logging on" \
    -ex "set logging file /dev/null" \
    -ex "attach $csgo" \
    -ex "set \$dlopen = (void*(*)(char*, int)) dlopen" \
    -ex "set \$dlclose = (int(*)(void*)) dlclose" \
    -ex "set \$library = \$dlopen(\"$(pwd)/Spartan.so\", 6)" \
    -ex "call \$dlclose(\$library)" \
    -ex "call \$dlclose(\$library)" \
    -ex "detach" \
    -ex "quit"

if ! grep -q "Spartan.so" /proc/$csgo/maps; then
    echo -e "$success_prefix Project Spartan has been successfully uninjected."
else
    echo -e "$error_prefix Project Spartan has failed to uninject. Close CS:GO to uninject."
fi

if [ -f "$(pwd)/gdb.txt" ]; then
    sudo rm -rf gdb.txt
fi
