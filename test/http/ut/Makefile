# Target name
TARGET = http

# Directories
OBJDIR = obj
SRCDIR = src
INCDIR = inc
DEPDIR = dep
BINDIR = bin
DEPDIR = .d

include $(PROJBASE)/cm/global.mk
include $(PROJBASE)/cm/filter_code.mk
include $(PROJBASE)/cm/bin_rules.mk
include $(PROJBASE)/test/gtest_rules.mk

# include
INC += -I$(INCDIR) -I$(PROJBASE)/cm/foundation/inc -I$(PROJBASE)/cm/thread/inc -I$(PROJBASE)/cm/util/inc -I$(PROJBASE)/cm/log/inc -I$(PROJBASE)/net/inc -I$(PROJBASE)/http/inc

# platform libs dependency 
LIBABOOST = /usr/local/boostx86/lib/libboost_system.a
LIBSOBOOST = -L/usr/local//boostx86/lib -lboost_system

LIBS += $(PROJBASE)/http/lib/libhttp.a $(PROJBASE)/net/lib/libnet.a $(PROJBASE)/cm/lib/libcm.a
LIBS += $(LIBAGTEST)
LIBS += $(LIBSA_LOG4CPLUS) $(LIBABOOST)

# compile flags

# link flags
LDFLAGS += -pthread

# link c or cpp target
LD := $(CXX)


