#===============================================================================
#                                                                               
#@par Classification:  UNCLASSIFIED, OPEN SOURCE                                
#                                                                               
#@program  Makefile for datr.open.source                                        
#                                                                               
#   Create by makemake with the following parameters:                           
#       --terse                                                                 
#       -sboost                                                                 
#       -D_REENTRANT                                                            
#       -d../common                                                             
#       -dsrc                                                                   
#       -std=c++0x                                                              
#       --temp-dir=./                                                           
#       -lpthread                                                               
#       -lso/usr/lib64/libboost_thread.so                                       
#       -lso/usr/lib64/libboost_regex.so                                        
#       -lrt                                                                    
#                                                                               
#@author    makemake    MM      Utility by DHF, last modified 2017-02-02        
#                                                                               
#@version   2020-09-21  MM      File creation                                   
#                                                                               
#===============================================================================

#-------------------------------------------------------------------------------
# Linux Configuration Flags                                                     
#-------------------------------------------------------------------------------
AR          :=  ar

RANLIB      :=  ranlib

CXX         :=  g++

CFLAGS      :=  -Wall           \
                -Wpointer-arith \
                -Wcast-align    \
                -O2             \
                -D_REENTRANT    

CXXFLAGS    :=  -Wall           \
                -Wpointer-arith \
                -Wcast-align    \
                -O2             \
                -D_REENTRANT    \
                -std=c++0x      

BINDIR      :=  ./bin

BINEXT      :=

OBJDIR      :=  ./obj

OBJEXT      :=  .o

INCPATH     :=  -I../common \
                -Isrc       

LIBS        :=  -ldatr.open.source            \
                -lpthread                     \
                /usr/lib64/libboost_thread.so \
                /usr/lib64/libboost_regex.so  \
                -lrt                          

LIBDIRS     :=  -L$(OBJDIR) 


#-------------------------------------------------------------------------------
#   debug configuration flags                                                   
#-------------------------------------------------------------------------------
ifneq ($(DEBUG),)
CFLAGS    := $(CFLAGS) -g -O0 -rdynamic
CXXFLAGS  := $(CXXFLAGS) -g -O0 -rdynamic -D_DEBUG
BINDIR    = ./debug
OBJDIR    = ./obj-debug
LIBDIRS   := -L./obj-debug -L$(OBJDIR) 
endif

#-------------------------------------------------------------------------------
#   Test object files                                                           
#-------------------------------------------------------------------------------
TESTOBJ =   \
  $(OBJDIR)/dev_test_work_test$(OBJEXT)  \
  $(OBJDIR)/lib_bits_work_test$(OBJEXT)  \
  $(OBJDIR)/lib_eu_work_metadatatest$(OBJEXT)  \
  $(OBJDIR)/lib_eu_work_valuetest$(OBJEXT)  \
  $(OBJDIR)/lib_ieee_ts754_work_floattest$(OBJEXT)  \
  $(OBJDIR)/lib_msg_conversionlabtest$(OBJEXT)  \
  $(OBJDIR)/lib_msg_publishertest$(OBJEXT)  \
  $(OBJDIR)/lib_msg_subscribertest$(OBJEXT)  \
  $(OBJDIR)/lib_work_filetest$(OBJEXT)  \
  $(OBJDIR)/lib_work_vectorwithoffsetcollectiontest$(OBJEXT)

.PHONY: all
all:    \
    $(BINDIR)/dev_test_main$(BINEXT)  \
    $(OBJDIR)/libdatr.open.source.a

#-------------------------------------------------------------------------------
#  file:  ../common/dev_test_main.cpp                                           
#-------------------------------------------------------------------------------
$(OBJDIR)/dev_test_main.o: ../common/dev_test_main.cpp ../common/dev_test_work.h  \
 ../common/dev_test_work_test.h ../common/lib_ds_shared_ptr.h  \
 ../common/lib_config_work_filepaths.h ../common/dev_debug.h  \
 ../common/lib_compiler_info.h ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


.PHONY: dev_test_main$(BINEXT)
dev_test_main$(BINEXT):  $(BINDIR)/dev_test_main$(BINEXT)

$(BINDIR)/dev_test_main$(BINEXT): $(OBJDIR)/dev_test_main$(OBJEXT) \
  $(OBJDIR)/libdatr.open.source.a 
	@ echo $@
	@ $(CXX) $(CXXFLAGS) $(LIBDIRS) -o "$@" $(OBJDIR)/dev_test_main$(OBJEXT) $(TESTOBJ) $(LIBS)
	@ ./.auto_runtest $(BINDIR)/dev_test_main


#-------------------------------------------------------------------------------
#  file:  ../common/lib_work_filetest.cpp                                       
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_work_filetest.o: ../common/lib_work_filetest.cpp  \
 ../common/lib_work_filetest.h ../common/dev_test_work_test.h  \
 ../common/lib_ds_shared_ptr.h ../common/dev_test_work.h  \
 ../common/lib_config_work_filepaths.h ../common/lib_compiler_info.h  \
 ../common/lib_file_work_info.h ../common/lib_log_work_exception.h  \
 ../common/lib_log_work_message.h ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_ds_flags.h  \
 ../common/lib_log_ds.h ../common/lib_macro.h ../common/lib_string.h  \
 ../common/lib_work_file.h ../common/debug.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_bits_work_test.cpp                                      
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_bits_work_test.o: ../common/lib_bits_work_test.cpp  \
 ../common/lib_bits_work_test.h ../common/dev_test_work_test.h  \
 ../common/lib_ds_shared_ptr.h ../common/dev_test_work.h  \
 ../common/lib_config_work_filepaths.h ../common/lib_bits_work.h  \
 ../common/debug.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_eu_work_value.cpp                                       
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_eu_work_value.o: ../common/lib_eu_work_value.cpp  \
 ../common/lib_eu_work_value.h ../common/lib_ds_shared_ptr.h  \
 ../common/lib_eu_work_metadata.h ../common/lib_eu_work_datatype.h  \
 ../common/lib_ds_enum.h ../common/lib_work_executebeforemain.h  \
 ../common/lib_compiler_info.h ../common/lib_string.h  \
 ../common/lib_bits_work.h ../common/debug.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_mp_work_threadinfo.cpp                                  
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_mp_work_threadinfo.o: ../common/lib_mp_work_threadinfo.cpp  \
 ../common/lib_mp_work_threadinfo.h ../common/lib_log_work_message.h  \
 ../common/lib_ds_shared_ptr.h ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_ds_flags.h  \
 ../common/lib_log_ds.h ../common/lib_work_namedobject.h  \
 ../common/lib_log_work_messagefactory.h ../common/lib_string.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_eu_work_metadata.cpp                                    
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_eu_work_metadata.o: ../common/lib_eu_work_metadata.cpp  \
 ../common/lib_eu_work_metadata.h ../common/lib_eu_work_datatype.h  \
 ../common/lib_ds_enum.h ../common/lib_work_executebeforemain.h  \
 ../common/lib_compiler_info.h ../common/lib_string.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_ieee_ts754_work_float.cpp                               
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_ieee_ts754_work_float.o: ../common/lib_ieee_ts754_work_float.cpp  \
 ../common/lib_ieee_ts754_work_float.h ../common/debug.h  \
 ../common/lib_bits_work.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_log_work_message.cpp                                    
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_log_work_message.o: ../common/lib_log_work_message.cpp  \
 ../common/lib_log_work_message.h ../common/lib_ds_shared_ptr.h  \
 ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_ds_flags.h  \
 ../common/lib_log_ds.h ../common/lib_log_work.h  \
 ../common/lib_si_ds_prefixes.h ../common/lib_string.h ../common/debug.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_work_file.cpp                                           
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_work_file.o: ../common/lib_work_file.cpp ../common/lib_work_file.h  \
 ../common/lib_log_work_message.h ../common/lib_ds_shared_ptr.h  \
 ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_ds_flags.h  \
 ../common/lib_log_ds.h ../common/lib_log_work_exception.h  \
 ../common/lib_macro.h ../common/lib_string.h  \
 ../common/lib_compiler_info.h ../common/lib_file_work_info.h  \
 ../common/lib_log_work_messagefactory.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_config_work_filepaths.cpp                               
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_config_work_filepaths.o: ../common/lib_config_work_filepaths.cpp  \
 ../common/lib_config_work_filepaths.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_log_ds.cpp                                              
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_log_ds.o: ../common/lib_log_ds.cpp ../common/lib_log_ds.h  \
 ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_work_version.cpp                                        
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_work_version.o: ../common/lib_work_version.cpp  \
 ../common/lib_work_version.h ../common/lib_string.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_ds_standardunits.cpp                                    
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_ds_standardunits.o: ../common/lib_ds_standardunits.cpp  \
 ../common/lib_ds_standardunits.h ../common/lib_ds_enum.h  \
 ../common/lib_work_executebeforemain.h ../common/lib_compiler_info.h  \
 ../common/lib_string.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_ieee_ts754_work_floattest.cpp                           
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_ieee_ts754_work_floattest.o:  \
 ../common/lib_ieee_ts754_work_floattest.cpp  \
 ../common/lib_ieee_ts754_work_floattest.h ../common/dev_test_work_test.h  \
 ../common/lib_ds_shared_ptr.h ../common/dev_test_work.h  \
 ../common/lib_config_work_filepaths.h  \
 ../common/lib_ieee_ts754_work_float.h ../common/debug.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/dev_test_work.cpp                                           
#-------------------------------------------------------------------------------
$(OBJDIR)/dev_test_work.o: ../common/dev_test_work.cpp ../common/dev_test_work.h  \
 ../common/dev_test_work_test.h ../common/lib_ds_shared_ptr.h  \
 ../common/lib_config_work_filepaths.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_msg_conversionlabtest.cpp                               
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_msg_conversionlabtest.o: ../common/lib_msg_conversionlabtest.cpp  \
 ../common/lib_msg_conversionlabtest.h ../common/dev_test_work_test.h  \
 ../common/lib_ds_shared_ptr.h ../common/dev_test_work.h  \
 ../common/lib_config_work_filepaths.h  \
 ../common/lib_mp_work_threadablecollection.h ../common/lib_cast.h  \
 ../common/lib_log_work_message.h ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_ds_flags.h  \
 ../common/lib_log_ds.h ../common/lib_mp_work_threadable.h  \
 ../common/lib_mp_work_thread.h  \
 ../common/lib_mp_work_threadinfo.h ../common/lib_work_namedobject.h  \
 ../common/lib_msg_publisher.h ../common/lib_ds_null.h  \
 ../common/lib_msg_subscriber.h ../common/lib_mp_work_queue.h  \
 ../common/lib_msg_publishersubscriberbase.h  \
 ../common/lib_msg_subscription.h ../common/lib_msg_subscriptionbase.h  \
 ../common/lib_msg_publication.h ../common/lib_msg_publisherbase.h  \
 ../common/lib_msg_publisherhelper.h  \
 ../common/lib_msg_publisherconnectionhelper.h  \
 ../common/lib_msg_actualconnection.h ../common/lib_string.h  \
 ../common/lib_msg_conversionlab.h ../common/lib_work_conversionlab.h  \
 ../common/lib_work_conversionlabbase.h ../common/debug.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_mp_work_threadablecollection.cpp                        
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_mp_work_threadablecollection.o:  \
 ../common/lib_mp_work_threadablecollection.cpp  \
 ../common/lib_mp_work_threadablecollection.h ../common/lib_cast.h  \
 ../common/lib_ds_shared_ptr.h ../common/lib_log_work_message.h  \
 ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_ds_flags.h  \
 ../common/lib_log_ds.h ../common/lib_mp_work_threadable.h  \
 ../common/lib_mp_work_thread.h  \
 ../common/lib_mp_work_threadinfo.h ../common/lib_work_namedobject.h  \
 ../common/lib_msg_publisher.h ../common/lib_ds_null.h  \
 ../common/lib_msg_subscriber.h ../common/lib_mp_work_queue.h  \
 ../common/lib_msg_publishersubscriberbase.h  \
 ../common/lib_msg_subscription.h ../common/lib_msg_subscriptionbase.h  \
 ../common/lib_msg_publication.h ../common/lib_msg_publisherbase.h  \
 ../common/lib_msg_publisherhelper.h  \
 ../common/lib_msg_publisherconnectionhelper.h  \
 ../common/lib_msg_actualconnection.h ../common/lib_string.h  \
 ../common/lib_log_work_messagefactory.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_string.cpp                                              
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_string.o: ../common/lib_string.cpp ../common/lib_compiler_info.h  \
 ../common/lib_string.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_log_work_exception.cpp                                  
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_log_work_exception.o: ../common/lib_log_work_exception.cpp  \
 ../common/lib_log_work_exception.h ../common/lib_log_work_message.h  \
 ../common/lib_ds_shared_ptr.h ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_ds_flags.h  \
 ../common/lib_log_ds.h ../common/lib_macro.h ../common/lib_string.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/dev_debug.cpp                                               
#-------------------------------------------------------------------------------
$(OBJDIR)/dev_debug.o: ../common/dev_debug.cpp ../common/dev_debug.h  \
 ../common/lib_compiler_info.h ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_string.h  \
 ../common/debug.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_time_work_datedeltatimebase.cpp                         
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_time_work_datedeltatimebase.o:  \
 ../common/lib_time_work_datedeltatimebase.cpp  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_si_ds_prefixes.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_eu_work_valuetest.cpp                                   
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_eu_work_valuetest.o: ../common/lib_eu_work_valuetest.cpp  \
 ../common/lib_eu_work_valuetest.h ../common/dev_test_work_test.h  \
 ../common/lib_ds_shared_ptr.h ../common/dev_test_work.h  \
 ../common/lib_config_work_filepaths.h ../common/lib_eu_work_value.h  \
 ../common/lib_eu_work_metadata.h ../common/lib_eu_work_datatype.h  \
 ../common/lib_ds_enum.h ../common/lib_work_executebeforemain.h  \
 ../common/lib_compiler_info.h ../common/lib_string.h ../common/debug.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_eu_work_metadatatest.cpp                                
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_eu_work_metadatatest.o: ../common/lib_eu_work_metadatatest.cpp  \
 ../common/lib_eu_work_metadatatest.h ../common/dev_test_work_test.h  \
 ../common/lib_ds_shared_ptr.h ../common/dev_test_work.h  \
 ../common/lib_config_work_filepaths.h ../common/lib_eu_work_metadata.h  \
 ../common/lib_eu_work_datatype.h ../common/lib_ds_enum.h  \
 ../common/lib_work_executebeforemain.h ../common/lib_compiler_info.h  \
 ../common/lib_string.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  src/lib_time_work_walltime.cpp                                        
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_time_work_walltime.o: src/lib_time_work_walltime.cpp  \
 src/lib_time_work_walltime.h ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_time_work_deltatime.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_bits_work.cpp                                           
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_bits_work.o: ../common/lib_bits_work.cpp ../common/lib_bits_work.h  \
 ../common/debug.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/debug.cpp                                                   
#-------------------------------------------------------------------------------
$(OBJDIR)/debug.o: ../common/debug.cpp ../common/debug.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_time_work_datetime.cpp                                  
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_time_work_datetime.o: ../common/lib_time_work_datetime.cpp  \
 ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_si_ds_prefixes.h  \
 ../common/lib_string.h ../common/lib_time_ds.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_work_vectorwithoffsetcollectiontest.cpp                 
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_work_vectorwithoffsetcollectiontest.o:  \
 ../common/lib_work_vectorwithoffsetcollectiontest.cpp  \
 ../common/lib_work_vectorwithoffsetcollectiontest.h  \
 ../common/dev_test_work_test.h ../common/lib_ds_shared_ptr.h  \
 ../common/dev_test_work.h ../common/lib_config_work_filepaths.h  \
 ../common/lib_pod_work_vectorwithoffsetfilereader.h  \
 ../common/lib_work_file.h ../common/lib_log_work_message.h  \
 ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_ds_flags.h  \
 ../common/lib_log_ds.h ../common/lib_log_work_exception.h  \
 ../common/lib_macro.h ../common/lib_string.h  \
 ../common/lib_ds_vectorwithoffset.h ../common/lib_ds_offset.h  \
 ../common/lib_work_vectorwithoffsetcollection.h  \
 ../common/lib_work_vectorwithoffsetcollection.tpp
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_file_work_info.cpp                                      
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_file_work_info.o: ../common/lib_file_work_info.cpp  \
 ../common/lib_file_work_info.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/dev_test_work_test.cpp                                      
#-------------------------------------------------------------------------------
$(OBJDIR)/dev_test_work_test.o: ../common/dev_test_work_test.cpp  \
 ../common/dev_test_work_test.h ../common/lib_ds_shared_ptr.h  \
 ../common/lib_compiler_info.h ../common/lib_string.h ../common/debug.h  \
 src/lib_time_work_walltime.h ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_mp_work_thread.cpp                                      
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_mp_work_thread.o: ../common/lib_mp_work_thread.cpp  \
 ../common/lib_mp_work_thread.h  \
 ../common/lib_ds_shared_ptr.h ../common/lib_mp_work_threadinfo.h  \
 ../common/lib_log_work_message.h ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_ds_flags.h  \
 ../common/lib_log_ds.h ../common/lib_work_namedobject.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_msg_publishertest.cpp                                   
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_msg_publishertest.o: ../common/lib_msg_publishertest.cpp  \
 ../common/lib_msg_publishertest.h ../common/dev_test_work_test.h  \
 ../common/lib_ds_shared_ptr.h ../common/dev_test_work.h  \
 ../common/lib_config_work_filepaths.h ../common/lib_msg_publisher.h  \
 ../common/lib_cast.h ../common/lib_ds_null.h  \
 ../common/lib_msg_subscriber.h ../common/lib_mp_work_queue.h  \
 ../common/lib_mp_work_threadable.h ../common/lib_mp_work_thread.h  \
 ../common/lib_mp_work_threadinfo.h ../common/lib_log_work_message.h  \
 ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_ds_flags.h  \
 ../common/lib_log_ds.h ../common/lib_work_namedobject.h  \
 ../common/lib_msg_publishersubscriberbase.h  \
 ../common/lib_msg_subscription.h ../common/lib_msg_subscriptionbase.h  \
 ../common/lib_msg_publication.h ../common/lib_msg_publisherbase.h  \
 ../common/lib_msg_publisherhelper.h  \
 ../common/lib_msg_publisherconnectionhelper.h  \
 ../common/lib_msg_actualconnection.h ../common/lib_string.h  \
 ../common/lib_ds_vectorwithoffset.h ../common/lib_ds_offset.h  \
 ../common/lib_mp_work_threadablecollection.h ../common/debug.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_log_work_messagefactory.cpp                             
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_log_work_messagefactory.o: ../common/lib_log_work_messagefactory.cpp  \
 ../common/lib_log_work_messagefactory.h  \
 ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_log_ds.h  \
 ../common/lib_log_work_message.h ../common/lib_ds_shared_ptr.h  \
 ../common/lib_ds_flags.h ../common/lib_string.h  \
 ../common/lib_work_version.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_log_work.cpp                                            
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_log_work.o: ../common/lib_log_work.cpp ../common/lib_log_work.h  \
 ../common/lib_log_ds.h ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_time_work_deltatime.cpp                                 
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_time_work_deltatime.o: ../common/lib_time_work_deltatime.cpp  \
 ../common/lib_time_work_deltatime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_datetime.h ../common/lib_si_ds_prefixes.h  \
 ../common/lib_string.h ../common/lib_time_ds.h ../common/debug.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_si_ds_prefixes.cpp                                      
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_si_ds_prefixes.o: ../common/lib_si_ds_prefixes.cpp  \
 ../common/lib_si_ds_prefixes.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@


#-------------------------------------------------------------------------------
#  file:  ../common/lib_msg_subscribertest.cpp                                  
#-------------------------------------------------------------------------------
$(OBJDIR)/lib_msg_subscribertest.o: ../common/lib_msg_subscribertest.cpp  \
 ../common/lib_msg_subscribertest.h ../common/dev_test_work_test.h  \
 ../common/lib_ds_shared_ptr.h ../common/dev_test_work.h  \
 ../common/lib_config_work_filepaths.h ../common/lib_msg_subscriber.h  \
 ../common/lib_ds_null.h ../common/lib_mp_work_queue.h  \
 ../common/lib_mp_work_threadable.h ../common/lib_mp_work_thread.h  \
 ../common/lib_mp_work_threadinfo.h ../common/lib_log_work_message.h  \
 ../common/lib_time_work_datetime.h  \
 ../common/lib_time_work_datedeltatimebase.h  \
 ../common/lib_time_work_deltatime.h ../common/lib_ds_flags.h  \
 ../common/lib_log_ds.h ../common/lib_work_namedobject.h  \
 ../common/lib_msg_publishersubscriberbase.h  \
 ../common/lib_msg_subscription.h ../common/lib_msg_subscriptionbase.h  \
 ../common/lib_msg_publisher.h ../common/lib_cast.h  \
 ../common/lib_msg_publication.h ../common/lib_msg_publisherbase.h  \
 ../common/lib_msg_publisherhelper.h  \
 ../common/lib_msg_publisherconnectionhelper.h  \
 ../common/lib_msg_actualconnection.h ../common/lib_string.h  \
 ../common/lib_mp_work_threadablecollection.h ../common/debug.h
	@ echo $@
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	@forbidden -s $@




#-------------------------------------------------------------------------------
#  library:                                                                     
#-------------------------------------------------------------------------------
$(OBJDIR)/libdatr.open.source.a: \
  $(OBJDIR)/debug$(OBJEXT)  \
  $(OBJDIR)/dev_debug$(OBJEXT)  \
  $(OBJDIR)/dev_test_work$(OBJEXT)  \
  $(OBJDIR)/dev_test_work_test$(OBJEXT)  \
  $(OBJDIR)/lib_bits_work$(OBJEXT)  \
  $(OBJDIR)/lib_bits_work_test$(OBJEXT)  \
  $(OBJDIR)/lib_config_work_filepaths$(OBJEXT)  \
  $(OBJDIR)/lib_ds_standardunits$(OBJEXT)  \
  $(OBJDIR)/lib_eu_work_metadata$(OBJEXT)  \
  $(OBJDIR)/lib_eu_work_metadatatest$(OBJEXT)  \
  $(OBJDIR)/lib_eu_work_value$(OBJEXT)  \
  $(OBJDIR)/lib_eu_work_valuetest$(OBJEXT)  \
  $(OBJDIR)/lib_file_work_info$(OBJEXT)  \
  $(OBJDIR)/lib_ieee_ts754_work_float$(OBJEXT)  \
  $(OBJDIR)/lib_ieee_ts754_work_floattest$(OBJEXT)  \
  $(OBJDIR)/lib_log_ds$(OBJEXT)  \
  $(OBJDIR)/lib_log_work$(OBJEXT)  \
  $(OBJDIR)/lib_log_work_exception$(OBJEXT)  \
  $(OBJDIR)/lib_log_work_message$(OBJEXT)  \
  $(OBJDIR)/lib_log_work_messagefactory$(OBJEXT)  \
  $(OBJDIR)/lib_mp_work_thread$(OBJEXT)  \
  $(OBJDIR)/lib_mp_work_threadablecollection$(OBJEXT)  \
  $(OBJDIR)/lib_mp_work_threadinfo$(OBJEXT)  \
  $(OBJDIR)/lib_msg_conversionlabtest$(OBJEXT)  \
  $(OBJDIR)/lib_msg_publishertest$(OBJEXT)  \
  $(OBJDIR)/lib_msg_subscribertest$(OBJEXT)  \
  $(OBJDIR)/lib_si_ds_prefixes$(OBJEXT)  \
  $(OBJDIR)/lib_string$(OBJEXT)  \
  $(OBJDIR)/lib_time_work_datedeltatimebase$(OBJEXT)  \
  $(OBJDIR)/lib_time_work_datetime$(OBJEXT)  \
  $(OBJDIR)/lib_time_work_deltatime$(OBJEXT)  \
  $(OBJDIR)/lib_time_work_walltime$(OBJEXT)  \
  $(OBJDIR)/lib_work_file$(OBJEXT)  \
  $(OBJDIR)/lib_work_filetest$(OBJEXT)  \
  $(OBJDIR)/lib_work_vectorwithoffsetcollectiontest$(OBJEXT)  \
  $(OBJDIR)/lib_work_version$(OBJEXT)  
	@ $(AR) ru $@ $?
	@ $(RANLIB) $@
	@ echo \$$1 --summary --not-impl --failed --slow --files $? > .auto_runtest
	@ chmod +x .auto_runtest

$(BINDIR):
	@mkdir -p $(BINDIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

.PHONY: debug
debug:
	@ $(MAKE) -f $(MAKEFILE_LIST) all DEBUG=1

.PHONY: clean
clean:
	@rm -f $(BINDIR)/* $(OBJDIR)/*
