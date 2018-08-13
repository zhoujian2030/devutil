# Target name
TARGET = tel

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
INC += -I$(INCDIR) -I$(PROJBASE)/tel/inc

# platform libs dependency 
LIBS += $(PROJBASE)/tel/lib/libtel.a 
LIBS += $(LIBAGTEST)

# compile flags

# link flags

# link c or cpp target
LD := $(CXX)

