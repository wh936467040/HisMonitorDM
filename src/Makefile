CC=gcc
CPP=g++ -g
dbg_option=
monitor_source=./
monitor_include=./
d5000_include=$(HOME)/src/include

COMMONDEF=-D_POSIX_REALTIME_SIGNALS -D_REENTRANT -DLINUX_AIO_NOTIFY_USING_THREAD -DLINUX_USING_QUEUED_IO
CFLAGS=-fpic $(dbg_option) -I $(monitor_include) -I $(d5000_include)
VPATH=$(monitor_source)

PlatformLibs=-lc -lm -lrt -lpthread -ldl 
D5000_LIB_PATH=$(HOME)/lib
MONITOR_LIBS= -lsend -ldcisg
%.o:%.cpp
	$(CPP) -c  $(CFLAGS) $< -o $@
%.o:%.cc
	$(CPP) -c  $(CFLAGS) $< -o $@

source_objects=log.o Parameter.o Runner.o alarmSet.o CollectLog.o getAndSendSession.o MetricDao.o MySendFactory.o\
		MyUtil.o Metric.o main.o

object_files=$(source_objects) 

#final_object=control

#all : $(final_object)

.PHONY : all clean rebuild 
	
exe2_his: $(object_files)
	$(CPP) -o $@ $(object_files) -L$(D5000_LIB_PATH) $(MONITOR_LIBS) $(PlatformLibs)
	@rm -rf $(object_files)	
	@echo make ok.

clean :	
	@rm -rf $(object_files)	
#	@rm -rf $(final_object)

rebuild	: clean	all
