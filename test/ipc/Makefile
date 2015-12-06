# Target  name
TARGET = pipesimulator 

# Directories
OBJDIR = obj
SRCDIR = src
DEPDIR = dep

# Libraries
INC += -I$(PROJBASE)/cm/inc -I$(PROJBASE)/ipc/src
LIBS = $(PROJBASE)/log/lib/liblog.a

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include $(PROJBASE)/cm/global.mk

all: $(TARGET)

# Targets
$(TARGET): buildrepo $(OBJS)
#	@echo "OBJS = $(OBJS)"
	@mkdir -p $(DEPDIR)
	@cp $(PROJBASE)/ipc/obj/Pipe.o $(DEPDIR)
	@cp $(PROJBASE)/ipc/obj/IPCLogger.o $(DEPDIR)
	$(CXX) $(OBJS) $(DEPDIR)/*.o -o $@ -lrt $(LIBS)
	@mv $(TARGET) $(OBJDIR)

clean:
	rm -rf ./bin/* $(OBJDIR) $(SRCDIR)/*.o

install:
	mv $(OBJDIR)/$(TARGET) ./bin
	
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
