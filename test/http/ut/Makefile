# Target  name
TARGET = testHttp

# Directories
OBJDIR = obj
SRCDIR = src
DEPDIR = dep
BINDIR = bin

# Libraries
# gtest include
INCGTEST = -I$(PROJBASE)/test/lib/gtest/include
INC += $(INCGTEST) -I$(PROJBASE)/cm/foundation/inc -I$(PROJBASE)/cm/thread/inc -I$(PROJBASE)/cm/util/inc -I$(PROJBASE)/cm/log/inc -I$(PROJBASE)/net/inc -I$(PROJBASE)/http/inc
# MUST place libnet.a ahead of libcm.a because libnet depends on libcm.a
LIBS = $(PROJBASE)/http/lib/libhttp.a $(PROJBASE)/net/lib/libnet.a $(PROJBASE)/cm/lib/libcm.a 

LIBAGTEST = $(PROJBASE)/test/lib/gtest/lib/libgtest.a $(PROJBASE)/test/lib/gtest/lib/libgtest_main.a
LIBSOGTEST = -L$(PROJBASE)/test/lib/gtest/lib -lgtest -lgtest_main

LIBABOOST = /usr/local/lib/libboost_system.a
LIBSOBOOST = -L/usr/local/lib -lboost_system

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include $(PROJBASE)/cm/global.mk

all: $(TARGET)

# Targets
$(TARGET): buildrepo $(OBJS)
	$(CXX) $(OBJS) -o $@ -lrt -pthread $(LIBABOOST) $(LIBAGTEST) $(LIBS)
	@mv $(TARGET) $(OBJDIR)

clean:
	rm -rf $(BINDIR) $(OBJDIR) $(SRCDIR)/*.o $(DEPDIR)

install:
	mkdir -p $(BINDIR)
	cp $(OBJDIR)/$(TARGET) $(BINDIR)
	
buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef
