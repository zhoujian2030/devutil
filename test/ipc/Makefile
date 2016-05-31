# Target  name
TARGET = pipesimulator 
PTHREAD_TGT = pthreadtest

# Directories
OBJDIR = obj
SRCDIR = src
DEPDIR = dep
BINDIR = bin

# Libraries
INC += -I$(PROJBASE)/cm/foundation/inc -I$(PROJBASE)/cm/thread/inc -I$(PROJBASE)/ipc/src
LIBS = $(PROJBASE)/cm/lib/libcm.a

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))
PIPEOBJS := obj/PipeMain.o
PTHREADOBJS := obj/ThreadMain.o obj/TestThread.o $(PROJBASE)/cm/thread/obj/Thread.o

include $(PROJBASE)/cm/global.mk

all: $(TARGET) $(PTHREAD_TGT)

# Targets
$(TARGET): buildrepo $(OBJS)
#	@echo "OBJS = $(OBJS)"
	@mkdir -p $(DEPDIR)
	@cp $(PROJBASE)/ipc/obj/Pipe.o $(DEPDIR)
	@cp $(PROJBASE)/ipc/obj/IPCLogger.o $(DEPDIR)
	$(CXX) $(PIPEOBJS) $(DEPDIR)/*.o -o $@ -lrt $(LIBS)
	@mv $(TARGET) $(OBJDIR)

$(PTHREAD_TGT):
	$(CXX) $(PTHREADOBJS) -o $@ -pthread
	@mv $(PTHREAD_TGT) $(OBJDIR)	

clean:
	rm -rf $(BINDIR) $(OBJDIR) $(SRCDIR)/*.o $(DEPDIR)

install:
	@mkdir -p $(BINDIR)
	mv $(OBJDIR)/$(TARGET) $(BINDIR)
	
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
