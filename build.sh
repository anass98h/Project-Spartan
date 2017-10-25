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
useClang=false

for i in "$@"; do
    case $1 in
        -d|--debug) target="DEBUG"; shift ;;
        -r|--release) target="RELEASE"; shift ;;
	    -c|--clang) useClang=true; shift ;;
	    -g|--gcc) useClang=false; shift ;;
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

    if [ "$useClang" = true ]; then
        if [ -n "$(uname -a | grep Ubuntu)" ]; then
            export CC=/usr/bin/clang-5.0
            export CXX=/usr/bin/clang++-5.0
        else
            export CC=/usr/bin/clang
            export CXX=/usr/bin/clang++
        fi
    else
        export CC=/usr/bin/gcc
        export CXX=/usr/bin/g++
    fi

    cmake -DCMAKE_BUILD_TYPE="$target" .
    if [ $? -ne 0 ]; then
        echo -e "$error_prefix Failed to create CMake files."
        exit -1
    fi

    if [ -z $CI ]; then
        echo -e "$prefix Building using $(grep "^processor" /proc/cpuinfo | wc -l) threads."

        make -j$(grep "^processor" /proc/cpuinfo | wc -l)
        if [ $? -ne 0 ]; then
            echo -e "$error_prefix Failed to build Project Spartan."
            exit -1
        fi
    else
        echo -e "$prefix CircleCI detected! Building using 2 threads."

        make -j 2
        if [ $? -ne 0 ]; then
            echo -e "$error_prefix Failed to build Project Spartan."
            exit -1
        fi
    fi

    if [ target != "DEBUG" ]; then
        strip -s libSpartan.so
        if [ $? -ne 0 ]; then
            echo -e "$error_prefix Failed to strip symbols from Spartan library file."
            exit -1
        fi
    fi

    if [ -z $CI ]; then
        echo -e "$prefix Renaming \e[2mlibSpartan.so\e[0m to \e[2mSpartan.so\e[0m."

        mv libSpartan.so Spartan.so
        if [ $? -ne 0 ]; then
            echo -e "$error_prefix Failed to move \e[2mlibSpartan.so\e[0m to \e[2mSpartan.so\e[0m. Please move manually."
            exit -1
        fi
    else
        if [ "$useClang" = true ]; then
            echo -e "$prefix CircleCI detected! Renaming \e[2mlibSpartan.so\e[0m to \e[2mSpartan-Clang.so\e[0m."

            mv libSpartan.so Spartan-Clang.so
            if [ $? -ne 0 ]; then
                echo -e "$error_prefix Failed to move \e[2mlibSpartan.so\e[0m to \e[2mSpartan.so\e[0m."
                exit -1
            fi
        else
            echo -e "$prefix CircleCI detected! Renaming \e[2mlibSpartan.so\e[0m to \e[2mSpartan-GCC.so\e[0m."

            mv libSpartan.so Spartan-GCC.so
            if [ $? -ne 0 ]; then
                echo -e "$error_prefix Failed to move \e[2mlibSpartan.so\e[0m to \e[2mSpartan.so\e[0m."
                exit -1
            fi
        fi
    fi

    patchelf --set-soname Spartan Spartan.so
    if [ $? -ne 0 ]; then
        echo -e "$error_prefix Failed to set .so name."
        while true; do
            echo -e -n "$error_prefix "
            read -p $'Do you wish to continue? (y/N) ' yn
            case $yn in
                [Yy]* ) break;;
                [Nn]* ) rm -rf Spartan.so; exit -1;;
                * ) echo -e "$error_prefix Please answer yes or no.";;
            esac
        done
    fi

    if [ "$useClang" = true ]; then
        echo -e "$success_prefix Successfully built Project Spartan with target \e[1m$target\e[0m using \e[1mClang\e[0m."
    else
        echo -e "$success_prefix Successfully built Project Spartan with target \e[1m$target\e[0m using \e[1mGCC\e[0m."
    fi
else
    echo -e "$error_prefix Building is only possible with the development version of Project Spartan."
fi
