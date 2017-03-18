include ../deps/makefiles/platform.mak

LIBRARIES += ../uri_parser ../light_state_machine ../state_machine_client ../deps/stlplus/subsystems ../deps/stlplus/containers ../deps/stlplus/portability

ifeq ($(RELEASE),on)
CXXFLAGS += -ggdb
endif

CXXFLAGS += -Wall -Wextra -std=c++11
CXXFLAGS += -I../deps/plog/include
LDFLAGS += -Wall
LOADLIBES += -lm

ifeq ($(RELEASE),on)
TARGETDIR := ../../bin/release
else
TARGETDIR := ../../bin/debug
endif

IMAGENAME := $(TARGETDIR)/uri-parser
include ../deps/makefiles/gcc.mak
