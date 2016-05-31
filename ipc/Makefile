# Target  name
TARGET = ipc 

# Directories
OBJDIR = obj
SRCDIR = src
BINDIR = bin

# Libraries
INC += -I$(PROJBASE)/cm/foundation/inc -I$(PROJBASE)/cm/thread/inc -I$(PROJBASE)/cm/log/inc
LIBS += $(PROJBASE)/cm/lib/libcm.a

# Files and folders
#SRCS    = $(shell find $(SRCDIR) -name '*.cpp')
#SRCDIRS = $(shell find . -name '*.cpp' | dirname {} | sort | uniq | sed 's/\/$(SRCDIR)//g' )
#OBJS   := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include ../cm/global.mk

all: $(TARGET)
exe: $(TARGET)

# Targets
$(TARGET): buildrepo $(OBJS)
#	@echo "SRCS = $(SRCS)"
	$(CXX) $(OBJS) -o $@ -lrt -pthread $(LIBS)
	@mv $(TARGET) $(OBJDIR)

clean:
	rm -rf $(BINDIR) $(OBJDIR) $(SRCDIR)/*.o

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
