# Target  name
TARGET=
LIBA = libcm.a

# Directories
OBJDIR = obj
SRCDIR = src

# Libraries
INC  = 
LIBS = 

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include global.mk
CFLAGS += -DJDEBUG

all: $(LIBA)

# Targets
$(LIBA): buildrepo $(OBJS)
	@cp -rf $(SRCDIR)/*.h ./inc/

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
