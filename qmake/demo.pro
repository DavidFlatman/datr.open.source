
        ########################################################################
        #   qmake project file built on 2020-10-20                                 
        #   Generated by makepro (v. 0.1.0) on 2020-10-20                           
        #   Command Line:                                                               
        #
        #   -d../qt                                                               
        #   -d../qt/images                                                        
        #   -I../common                                                           
        #   -q../qt/datr                                                          
        #   -qtwidgets                                                            
        #   --qmake=qmake-qt4                                                     
        #   -f-std=c++11                                                          

        ########################################################################   
        
CONFIG          += largefile
CONFIG          += qt
CONFIG          += stl
CONFIG          += thread
CONFIG          += warn_off
CONFIG          += x11

QT              += widgets

QMAKE_CXXFLAGS  += -std=c++11

INCLUDEPATH     += ../common


DESTDIR         =   bin
OBJECTS_DIR     =   obj
MOC_DIR         =   moc



SOURCES         += ../qt/demo.cpp



