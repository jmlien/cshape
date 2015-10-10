
# turn on/off open GL display
# you will need to install freeglut on linux to make

OPENGL_ON := 1


#-----------------------------------------------------------------------------
# On Linux,    this should be 'Linux'
# On Mac OS X, this should be 'Darwin'
#-----------------------------------------------------------------------------

OSNAME := $(shell uname -s)

ifeq ($(OSNAME),Linux)
  PLATFORM := LINUX
else ifeq ($(OSNAME),Darwin)
  PLATFORM := MAC_OS_X
else
  PLATFORM := OTHER
endif

#-----------------------------------------------------------------------------
# Choose a compiler & its options
#--------------------------------------------------------------------------

CC       = gcc
CXX      = g++
LINK     = g++

OPTS = -std=c99 -MMD -Wall -O3
SUBDIRS = . qhull

#--------------------------------------------------------------------
# Xlib and OpenGL
#--------------------------------------------------------------------
ifneq ($(OPENGL_ON),0)
	OPTS += -DOPENGL_ON=1
	SUBDIRS += GL
	ifeq ($(PLATFORM),MAC_OS_X)
	   OPTS += -DMACOS
	   GL_LIB = -framework OpenGL -framework GLUT -framework Foundation
	else
	   X_LIB  = -Wl,-rpath,/usr/X11R6/lib -L/usr/X11R6/lib -L/usr/local/lib -lXext -lX11 -lXt 
	   GL_LIB = -lglut -lGLU -lGL -lm
	endif
endif 


#--------------------------------------------------------------------
# Qhull
#--------------------------------------------------------------------

ifeq ($(PLATFORM),MAC_OS_X)
  QHULL_LIB = -Lqhull -lqhull.osx
else
  QHULL_LIB = -Lqhull -lqhull.linux64  
endif

#--------------------------------------------------------------------
# Put all together
#--------------------------------------------------------------------

INCLUDE = $(addprefix -I,$(SUBDIRS)) 
SRCS=$(wildcard $(addsuffix /*.c,$(SUBDIRS)))
OBJS=${SRCS:.c=.o}
LIB = $(X_LIB) $(GL_LIB) $(QHULL_LIB)
CFLAGS   = $(OPTS) $(INCLUDE)
CXXFLAGS = $(CFLAGS) -Wno-deprecated

TARGET = shape


all: $(TARGET)

#--------------------------------------------------------------------	
objs: ${OBJS}
#--------------------------------------------------------------------

$(TARGET): objs 
	${CC} ${CFLAGS} -o $@ $(OBJS) $(LIB)

clean:
	rm -f *.o $(OBJS) $(TARGET) Dependencies 

#--------------------------------------------------------------------
.SUFFIXES: .c

.c.o:
	${CC} ${CFLAGS} -c $< -o $@
	cat $*.d >> Dependencies
	rm -f $*.d 

Dependencies:
	touch Dependencies

include Dependencies
