# datr.open.source/linux

This directoy contains the build procedures and output files for 
building under the Linux operating system.  In general, a user should
do the following to build the project:
    % cd datr.open.source/linux
    % ./configure.sh
    % make -j


## ./configure.sh

    The configure.sh script will build a Makefile that will build the 
    datr.open.source code base.

## Makefile
    
    The Makefile is created by configure.sh and should not be modified by
    hand.

## git
    
    Note:   The .gitignore file will block the linux/bin and linux/obj 
            directories from being placed in the git repository.
