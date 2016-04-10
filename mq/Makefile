# Target  name
TARGET=
LIBA = libmq.a

# Directories
OBJDIR = obj
SRCDIR = src

# Libraries
INC += -I$(PROJBASE)/cm/inc
LIBS = 

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include ../cm/global.mk

all: $(LIBA)

# Targets
$(LIBA): buildrepo $(OBJS)
	ar q $@ $(OBJS)
	@mv $(LIBA) ./lib
	@cp $(SRCDIR)/*.h ./inc/

clean:
	rm -rf ./lib/* ./inc/* $(OBJDIR) $(SRCDIR)/*.o
	
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
