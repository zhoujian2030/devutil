# Target  name
TARGET = dbtest 

# Directories
OBJDIR = obj
SRCDIR = src

# Libraries
INC += -I$(PROJBASE)/cm/inc -I$(PROJBASE)/log/inc
LIBS += $(PROJBASE)/log/lib/liblog.a

# Files and folders
#SRCS    = $(shell find $(SRCDIR) -name '*.cpp')
#SRCDIRS = $(shell find . -name '*.cpp' | dirname {} | sort | uniq | sed 's/\/$(SRCDIR)//g' )
#OBJS   := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

DEPOBJS := $(PROJBASE)/cm/obj/*.o

include ../cm/global.mk

all: $(TARGET)
exe: $(TARGET)

# Targets
$(TARGET): buildrepo $(OBJS)
#	@echo "SRCS = $(SRCS)"
	$(CXX) $(OBJS) $(DEPOBJS) -o $@ -lrt -lpq $(LIBS)
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
