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
    rm -rf Spartan.so
    if [ $? -ne 0 ]; then
        echo -e "$error_prefix Failed to delete old Project Spartan file."
        exit -1
    fi

    curlpp-config --version
    if [ $? -ne 0 ]; then
        echo -e "$prefix Could not find Curl++. Installing it as its a required dependency."
        bash -c "./install-curlpp.sh"
        if [ $? -ne 0 ]; then
            echo -e "$error_prefix Could not install Curl++. Please install it manually to proceed building."
            exit -1
        fi
    else
        echo -e "$prefix Successfully detected $(curlpp-config --version)."
    fi

    cmake -DCMAKE_BUILD_TYPE="$target" .
    if [ $? -ne 0 ]; then
        echo -e "$error_prefix Failed to create CMake files."
        exit -1
    fi

    make -j$(grep "^processor" /proc/cpuinfo | wc -l)
    if [ $? -ne 0 ]; then
        echo -e "$error_prefix Failed to build Project Spartan."
        exit -1
    fi

    if [ target != "DEBUG" ]; then
        strip -s libSpartan.so
        if [ $? -ne 0 ]; then
            echo -e "$error_prefix Failed to strip symbols from Spartan library file."
            exit -1
        fi
    fi

    mv libSpartan.so Spartan.so
    if [ $? -ne 0 ]; then
        echo -e "$error_prefix Failed to move \e[2mlibSpartan.so\e[0m to \e[2mSpartan.so\e[0m. Please move manually."
        exit -1
    fi

    echo -e "$success_prefix Successfully built Project Spartan with target \e[2m$target\e[0m."
else
    echo -e "$error_prefix Building is only possible with the development version of Project Spartan."
fi
