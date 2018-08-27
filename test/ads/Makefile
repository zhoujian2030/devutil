# Target name
TARGET = ads

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
INC += -I$(INCDIR) -I$(PROJBASE)/cm/foundation/inc -I$(PROJBASE)/ads/inc -I$(PROJBASE)/cm/clog/inc -I$(PROJBASE)/cm/3pp/inc -I$(PROJBASE)/test/cm/ut/inc

# platform libs dependency 
LIBS += $(PROJBASE)/ads/lib/libads.a $(PROJBASE)/cm/lib/libcm.a
LIBS += $(LIBAGTEST)

# compile flags


# link flags
LDFLAGS += -pthread

# link c or cpp target
LD := $(CXX)

