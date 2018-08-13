# Target name
TARGET = ads

# Directories
OBJDIR = obj
SRCDIR = src
INCDIR = inc
LIBDIR = lib
DEPDIR = .d

include $(PROJBASE)/cm/global.mk
include $(PROJBASE)/cm/filter_code.mk
include $(PROJBASE)/cm/bin_rules.mk

# include
INC += -I$(INCDIR) -I$(PROJBASE)/cm/foundation/inc -I$(PROJBASE)/cm/log/inc

# platform dependency
ifeq ($(PLATFORM), arm)
LIBS += $(PROJBASE)/cm/lib/libcm_arm.a
else
LIBS += $(PROJBASE)/cm/lib/libcm.a
endif
LIBS += $(LIBSA_LOG4CPLUS)

# compile flags

# link flags

# link c or cpp target
LD := $(CXX)

