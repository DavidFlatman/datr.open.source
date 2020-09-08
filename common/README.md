# datr.open.source/common

    This directory structure holds the source code that is expected to 
    compile and be used under both Linux and Windows.  

    It is perferred that the code have minimal amount of 
        #if IS_WINDOWS
            // windows code
        #else
            // non-windows code
        #endif


    There is a corresponding directory linux/src that holds files that are
    only expected to work under Linux.

