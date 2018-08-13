# Target name
TARGET = log

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
INC += -I$(INCDIR) -I$(PROJBASE)/cm/foundation/inc -I$(PROJBASE)/cm/log/inc

# platform libs dependency 
LIBS += $(PROJBASE)/cm/lib/libcm.a
LIBS += $(LIBSA_LOG4CPLUS)

# compile flags

# link flags
LDFLAGS += -pthread

# link c or cpp target
LD := $(CXX)

