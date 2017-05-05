# Target  name
TARGET = netserver 

# Directories
OBJDIR = obj
SRCDIR = src
DEPDIR = dep
BINDIR = bin

# Libraries
INC += -I$(PROJBASE)/cm/foundation/inc -I$(PROJBASE)/cm/thread/inc -I$(PROJBASE)/cm/log/inc -I$(PROJBASE)/net/inc
# MUST place libnet.a ahead of liblog.a because libnet depends on liblog.a
LIBS = $(PROJBASE)/net/lib/libnet.a $(PROJBASE)/cm/lib/libcm.a

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include $(PROJBASE)/cm/global.mk

all: $(TARGET)

# Targets
$(TARGET): buildrepo $(OBJS)
ifeq ($(SCTP_SUPPORT), TRUE)
	$(CXX) $(OBJS) -o $@ -lrt -lsctp -pthread $(LIBS)
else
	$(CXX) $(OBJS) -o $@ -lrt -pthread $(LIBS)
endif
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
