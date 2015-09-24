CC=g++

CFLAGS =-Wall -g  
LIBS = -lpthread -lm -lrt -std=c++11 -L/usr/local/mysql-5.5.44-linux2.6-i686/lib/ -lmysqlclient
INCLUDES=-I../include -I/usr/local/mysql-5.5.44-linux2.6-i686/include/ -I./include 
TARGET=IntelligenceClass
SOURCE := $(wildcard src/*.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCE))
MODULES += main pool source homework handle database_interface
ALLOBJS := $(addsuffix /src/*.o,$(MODULES))