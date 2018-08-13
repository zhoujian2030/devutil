# Target name
LIBNAME = libnet

# Directories
OBJDIR = obj
SRCDIR = src
INCDIR = inc
LIBDIR = lib
DEPDIR = .d

include $(PROJBASE)/cm/global.mk
include $(PROJBASE)/cm/filter_code.mk
include $(PROJBASE)/cm/lib_rules.mk

# Libraries
INC += -I$(INCDIR) -I$(PROJBASE)/cm/foundation/inc -I$(PROJBASE)/cm/thread/inc -I$(PROJBASE)/cm/log/inc

ifeq ($(SCTP_SUPPORT), TRUE)
CFLAGS += -DSCTP_SUPPORT
endif

