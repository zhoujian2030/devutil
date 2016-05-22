# Target  name
TGT_VARIADIC = add-demo 

# Directories
OBJDIR = obj
SRCDIR = src

# Libraries
INC += -I$(PROJBASE)/cm/foundation/inc -I$(PROJBASE)/cm/log/inc
LIBS = 

# Files and folders
#SRCS    = $(shell find $(SRCDIR) -name '*.cpp')
#SRCDIRS = $(shell find . -name '*.cpp' | dirname {} | sort | uniq | sed 's/\/$(SRCDIR)//g' )
#OBJS   := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include ../cm/global.mk

all: $(TGT_VARIADIC)
variadic: $(TGT_VARIADIC)

# Targets
$(TGT_VARIADIC): buildrepo $(OBJS)
#	@echo "SRCS = $(SRCS)"
	$(CXX) $(OBJS) $(LIBS) -o $@
	@mv $(TGT_VARIADIC) $(OBJDIR)

clean:
	rm -rf ./bin/* $(OBJDIR) $(SRCDIR)/*.o

install:
	mv $(OBJDIR)/$(TGT_VARIADIC) ./bin
	
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
