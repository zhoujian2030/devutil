# Target  name
TARGET = rpc 

# Directories
OBJDIR = obj
SRCDIR = src

# Libraries
INC += -I$(PROJBASE)
LIBS = 

# Files and folders
#SRCS    = $(shell find $(SRCDIR) -name '*.cpp')
#SRCDIRS = $(shell find . -name '*.cpp' | dirname {} | sort | uniq | sed 's/\/$(SRCDIR)//g' )
#OBJS   := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include rpc.mk

all: $(TARGET)
exe: $(TARGET)

# Targets
$(TARGET): buildrepo $(OBJS)
#	@echo "SRCS = $(SRCS)"
	$(CXX) $(OBJS) $(LIBS) -o $@
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
