# Target  name
TARGET = pipesimulator 

# Directories
OBJDIR = obj
SRCDIR = src

# Libraries
INC += -I$(PROJBASE)/cm/inc -I$(PROJBASE)/ipc/src
LIBS = 

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include $(PROJBASE)/cm/global.mk

all: $(TARGET)

# Targets
$(TARGET): buildrepo $(OBJS)
#	@echo "OBJS = $(OBJS)"
	@cp $(PROJBASE)/ipc/obj/Pipe.o $(OBJDIR)
	$(CXX) $(OBJS) $(OBJDIR)/Pipe.o $(LIBS) -o $@
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
