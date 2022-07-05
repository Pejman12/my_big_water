#************************************************
#*                                              *
#*             (c) 2017 J. FABRIZIO             *
#*                                              *
#*                               LRDE EPITA     *
#*                                              *
#************************************************

CC = g++

CPP_FILES = src/engine/program.cc src/engine/object.cc src/reader/obj_raw.cc
CPP_FILES += src/reader/obj_loader.cc src/engine/camera.cc src/engine/scene.cc
CPP_FILES += src/water/water.cc src/water/waterFBO.cc
HXX_FILES = src/engine/program.hh src/engine/object.hh src/reader/obj_raw.hh
HXX_FILES += src/reader/obj_loader.hh src/engine/camera.hh src/engine/scene.hh
HXX_FILES += src/water/water.hh src/water/waterFBO.hh
OBJ_FILES = $(CPP_FILES:.cc=.o)

CXX_FLAGS += -Wall -Wextra -pedantic -std=c++17
CXX_FLAGS += -m64 -march=native
CXX_FLAGS += -ftree-vectorize #-fopt-info-vec-optimized -fopt-info-vec-missed
CXX_FLAGS += -Isrc/engine -Isrc/reader -Isrc/water
DEBUG_FLAGS += -DDEBUG -g -O0
RELEASE_FLAGS += -DNDEBUG -O3
LDXX_FLAGS = -lGL  -lGLEW -lglut -lpthread

MAIN_FILE = src/main.cc
DIST = main

SKEL_FILES = $(CPP_FILES) $(HXX_FILES) $(MAIN_FILE)


#For gcc 4.9
#CXXFLAGS+=-fdiagnostics-color=auto
export GCC_COLORS=1

define color
    if test -n "${TERM}" ; then\
	if test `tput colors` -gt 0 ; then \
	    tput setaf $(1); \
        fi;\
    fi
endef

define default_color
    if test -n "${TERM}" ; then\
	if test `tput colors` -gt 0 ; then  tput sgr0 ; fi; \
    fi
endef


all: CXX_FLAGS += $(RELEASE_FLAGS)
all: post-build

debug: CXX_FLAGS += $(DEBUG_FLAGS)
debug: post-build

pre-build:
	@$(call color,4)
	@echo "******** Starting Compilation ************"
	@$(call default_color)

post-build: main-build
	sta=$$?;	  \
	$(call color,4); \
	echo "*********** End Compilation **************"; \
	$(call default_color); \
	exit $$sta;

main-build: pre-build build

build: $(OBJ_FILES)
	$(CC) $(MAIN_FILE) -o $(DIST) $(OBJ_FILES) $(CXX_FLAGS) $(LDXX_FLAGS)


%.o: %.cc %.hh
	@$(call color,2)
	@echo "[$@] $(CXX_FLAGS)"
	@$(call default_color)
	@$(CC) -c -o $@ $< $(CXX_FLAGS) ; \
	sta=$$?;	  \
	if [ $$sta -eq 0 ]; then  \
	  $(call color,2) ; \
	  echo "[$@ succes]" ; \
	  $(call default_color) ; \
	else  \
	  $(call color,1) ; \
	  echo "[$@ failure]" ; \
	  $(call default_color) ; \
	fi ;\
	exit $$sta

.PHONY: all clean pre-build post-build main-build build

clean:
	rm -f $(OBJ_FILES)
	rm -f $(DIST)