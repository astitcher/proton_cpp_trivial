# Set up to get C++11
CXXFLAGS += -std=c++11 -Wall -pedantic

PROTON_INSTALL_PREFIX := ~/Work/qpid-install
PROTON_PKG_CONFIG := $(PROTON_INSTALL_PREFIX)/lib64/pkgconfig

# Set up to find the proton-cpp includes/libs in their installed area
PROTON_CXXFLAGS := $(shell PKG_CONFIG_PATH=$(PROTON_PKG_CONFIG) pkg-config --cflags libqpid-proton-cpp)
PROTON_LDFLAGS  := $(shell PKG_CONFIG_PATH=$(PROTON_PKG_CONFIG) pkg-config  --libs libqpid-proton-cpp)
PROTON_RPATHFLAGS  := -Wl,-rpath=$(shell PKG_CONFIG_PATH=$(PROTON_PKG_CONFIG) pkg-config  --variable=libdir libqpid-proton-cpp)

CXXFLAGS += $(PROTON_CXXFLAGS)
LDFLAGS += $(PROTON_LDFLAGS) $(PROTON_RPATHFLAGS)

# Just build everything as if it's a single file program
SOURCES := $(wildcard *.cpp)
EXES := $(patsubst %.cpp,%,$(SOURCES))

all: $(EXES)

clean: ;-rm -f $(EXES)

