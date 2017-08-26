#!/usr/bin/env bash

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
csgo_bin64=$(realpath ~/.steam/steam/steamapps/common/Counter-Strike\ Global\ Offensive/bin/linux64)
dir=$(pwd)

if [ ! -z "$csgo" ]; then
    echo -e "$error_prefix Please close CS:GO before replacing a CS:GO asset file."
    exit -1
fi

if [ -d ".git" ]; then
    echo -e "$prefix Detected development version of Project Spartan."
fi

if [ $# -eq 1 ]; then
    if [ -f "$dir/backup/binkalnx64.asi" ]; then
        rm -rf "${csgo_bin64}/binkalnx64.asi"
        cp -v "$dir/backup/binkalnx64.asi" "${csgo_bin64}/binkalnx64.asi" > /dev/null
	    echo -e "$success_prefix Successfully undone all changes."
	    exit 0
	else
	    echo -e "$error_prefix The Backup file doesn't exist."
	    exit -1
    fi
fi

if [ ! -d "backup" ]; then
    mkdir $(pwd)/backup
fi

if [ ! -f "$dir/backup/binkalnx64.asi" ]; then
    cp -v "${csgo_bin64}/binkalnx64.asi" "$dir/backup/binkalnx64.asi" > /dev/null
    echo -e "$success_prefix Successfully backed up \e[2mbinkalnx64.asi\e[0m file."
fi

rm -rf "${csgo_bin64}/binkalnx64.asi"

echo -e "void _init() { dlopen(L, 1); } " | gcc -shared -fPIC -s -O3 -nostdlib -DL=\"$(pwd)/Spartan.so\" -include dlfcn.h -o "${csgo_bin64}/binkalnx64.asi" -xc -

if [ $? -eq 0 ]; then
    echo -e "$success_prefix Successfully replaced \e[2mbinkalnx64.asi\e[0m file with Spartan."
    echo -e "$success_prefix Project Spartan will be automaticly injected on the next launch of CS:GO."
else
    cp -v "$dir/backup/binkalnx64.asi" "${csgo_bin64}/binkalnx64.asi" > /dev/null
    echo -e "$error_prefix A error occured. All changes have been undone."
fi
