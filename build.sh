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

target="RELEASE"

for i in "$@"; do
    case $1 in
        -d|--debug) target="DEBUG"; shift ;;
        -r|--release) target="RELEASE"; shift ;;
        -rwd|-rd|--releasewithdebug|--releasewithdebuginfo) target="RELWITHDEBINFO"; shift ;;
    esac
    shift
done

if [ -d ".git" ]; then




else
    echo -e "$error_prefix Building is only possible with the development version of Project Spartan."
fi
