# Target  name
TARGET=

# Directories
OBJDIR = obj
SRCDIR = src
INCDIR = inc

# Libraries
INC  = -I$(PROJBASE)/cm/log/inc 
LIBS = 

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include ../global.mk
CFLAGS += -DDEBUG_CM

all: buildrepo $(OBJS)
	@mkdir -p $(INCDIR)
	@cp $(SRCDIR)/*.h $(INCDIR)

clean:
	rm -rf $(INCDIR) $(OBJDIR) $(SRCDIR)/*.o

install:
	# nothing to do 
	
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
