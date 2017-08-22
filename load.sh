#!/bin/sh

prefix="\e[34m\e[1mSpartan >\e[0m"
error_prefix="\e[91m\e[1mSpartan >\e[0m"

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
    echo -e "$error_prefix Please open Counter-Strike: Global Offensive."
    exit -1
fi

# Credit to aixxe from AimTuxOfficial
if grep -q "Spartan.so" /proc/$csgo/maps; then
    echo -e "$error_prefix Project Spartan has already been injected."
    exit -1
fi

if [ -d ".git" ]; then
    echo -e "$prefix Detected development version of Project Spartan."
fi

# Allows only root to use ptrace. This is temporary until the user reboots the machine.
sudo echo "2" | sudo tee /proc/sys/kernel/yama/ptrace_scope

# Prevent crash dumps from being sent to kisak
sudo rm -rf /tmp/dumps
sudo mkdir /tmp/dumps
sudo chmod 000 /tmp/dumps

# Credit to LWSS for removing the "warning: Could not load shared library symbols for bin/linux64/launcher_client.so."
input="$(
sudo gdb -n -q -batch-silent \
  -ex "set logging on" \
  -ex "set logging file /dev/null" \
  -ex "attach $csgo" \
  -ex "set \$dlopen = (void*(*)(char*, int)) dlopen" \
  -ex "call \$dlopen(\"/Spartan.so\", 1)" \
  -ex "detach" \
  -ex "quit"
)"

last_line="${input##*$'\n'}"

if [ "$last_line" != "\$1 = (void *) 0x0" ]; then
    echo -e "$prefix Project Spartan has been successfully injected."
else
    echo -e "$error_prefix Project Spartan has failed to inject. Please restart CS:GO and retry."
fi

if [ -f "$(pwd)/gdb.txt" ]; then
    sudo rm -rf gdb.txt
fi
