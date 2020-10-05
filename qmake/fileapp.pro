################################################################################
#   qmake project file built on 2020-10-05                                     
#   Generated by makepro (v. 0.1.0) on 2020-10-05                              
#   Command Line:                                                               
#      -m../qt/fileapp.cpp                                                     
#      -d../qt                                                                 
#      -d../qt/images                                                          
#      -I../common                                                             
#      -f-std=c++11                                                            
#      -qtwidgets                                                              
#      -q../qt/datr                                                            
################################################################################
CONFIG          += largefile
CONFIG          += qt
CONFIG          += stl
CONFIG          += thread
CONFIG          += warn_off
CONFIG          += x11

QT              += core
QT              += gui
QT              += widgets

QMAKE_CXXFLAGS  += -std=c++11

INCLUDEPATH     += ../qt
INCLUDEPATH     += ../qt/images
INCLUDEPATH     += ../common


DESTDIR         =   bin
OBJECTS_DIR     =   obj
MOC_DIR         =   moc


HEADERS         += ../qt/lib_qt_datetimepicker.h
HEADERS         += ../qt/lib_qt_pathrememberfield.h
HEADERS         += ../qt/lib_qt_lineeditrememberfield.h
HEADERS         += ../qt/lib_qt_rememberfield.h
HEADERS         += ../qt/lib_qt_formatselectorrememberfield.h
HEADERS         += ../qt/lib_qt_datetimefield.h
HEADERS         += ../qt/lib_qt_pathfield.h
HEADERS         += ../qt/lib_qt_field.h

SOURCES         += ../qt/fileapp.cpp
SOURCES         += ../qt/lib_qt_datetimefield.cpp
SOURCES         += ../qt/lib_qt_lineeditrememberfield.cpp
SOURCES         += ../qt/lib_qt_formatselectorrememberfield.cpp
SOURCES         += ../qt/lib_qt_pathrememberfield.cpp
SOURCES         += ../qt/lib_qt_pathfield.cpp
SOURCES         += ../qt/lib_qt_rememberfield.cpp
SOURCES         += ../qt/lib_qt_datetimepicker.cpp
SOURCES         += ../qt/lib_qt_field.cpp


