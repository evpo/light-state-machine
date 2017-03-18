include ../deps/makefiles/platform.mak

LIBRARIES += ../src

LDFLAGS += -Wl,--whole-archive
LOADLIBES += -Wl,--no-whole-archive

ifeq ($(RELEASE),on)
CXXFLAGS += -ggdb
endif

LOADLIBES += ../deps/gtest/googletest/build/libgtest.a -lpthread -lm
CXXFLAGS += -I../deps/gtest/googletest/include -pthread -std=c++11 -Wall -Wextra
CXXFLAGS += -I../include -I../include/internal -I../deps/stlplus/containers
CXXFLAGS += -I../deps/plog/include

LDFLAGS += -Wall

ifeq ($(RELEASE),on)
TARGETDIR := ../../bin/release
else
TARGETDIR := ../../bin/debug
endif

IMAGENAME := $(TARGETDIR)/uri-parser-tests
include ../deps/makefiles/gcc.mak
