#!/bin/bash

#-------------------------------------------------------------------------------
#                                                                               
#   Name: makeconfig                                                            
#                                                                               
#   Purpose:    Create the .pro and Makefile files for the known main       
#               programs.                                                   
#                                                                               
#               This script should be executed everytime that a new main.cpp is 
#               created.                                                        
#                                                                               
#   Note:       This file is created and maintained by hand.                    
#                                                                               
#   Note:       We would prefer to use Qt v5, but some of our developers are    
#               trapped at Qt v4 (and they don't have control over their        
#               development environment).                                       
#                                                                               
#   @version    2016-04-04  DHF     File creation.                              
#                                                                               
#   @version    2017-08-22  DHF     Added libcrypto.so                          
#                                                                               
#   @version    unknown     unknown Major overhaul                              
#                                                                               
#   @version    2020-09-30  MNT     Added QMAKE                                 
# 
#   @version    2020-10-19  PN      Combined makeconfig and configure.sh
#                                   Renamed makeconfig to configure.sh
#                                                                            
#-------------------------------------------------------------------------------

OUTPUT_DIR=../../../datr-non-cm/linux/
BOOST=${OUTPUT_DIR}boost/
BOOST_LIB=${BOOST}lib/

#-------------------------------------------------------------------------------
#   QMAKE                                                                       
#-------------------------------------------------------------------------------
QMAKE=qmake-qt4

if ! command -v $QMAKE &> /dev/null
then
    QMAKE=qmake
fi
        
#-------------------------------------------------------------------------------
#   make.qt.config                                                              
#-------------------------------------------------------------------------------
../tools/make.qt.config                                     \
    -d../qt                                                 \
    -d../qt/images                                          \
    -I../common                                             \
    -q../qt/datr                                            \
    -qtwidgets                                              \
    --qmake=$QMAKE                                          \
    -f-std=c++11                                            

chmod u+x build

