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

git clone https://github.com/jpbarrette/curlpp.git
if [ $? -ne 0 ]; then
    echo -e "$error_prefix Could not clone Curl++."
    exit -1
fi

cd curlpp
if [ $? -ne 0 ]; then
    echo -e "$error_prefix What the fuck?"
    exit -1
fi

cmake .
if [ $? -ne 0 ]; then
    echo -e "$error_prefix Could not create Curl++ Makefiles."
    exit -1
fi

make -j$(grep "^processor" /proc/cpuinfo | wc -l)
if [ $? -ne 0 ]; then
    echo -e "$error_prefix Could not build Curl++."
    exit -1
fi

sudo make install
if [ $? -ne 0 ]; then
    echo -e "$error_prefix Could not install Curl++."
    exit -1
fi

echo -e "$success_prefix Successfully built and installed Curl++."
