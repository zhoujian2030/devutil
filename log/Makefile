# Target  name
TARGET=
LIBA = liblog.a

# Directories
OBJDIR = obj
SRCDIR = src
LIBOBJDIR = liblog4cplus

# Libraries
INC += -I$(PROJBASE)/cm/inc
LIBS = /usr/local/lib/liblog4cplus.a

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include ../cm/global.mk

all: $(LIBA)

# Targets
$(LIBA): buildrepo $(OBJS)
	@mkdir -p $(LIBOBJDIR)
	ar x $(LIBS) 
	@mv *.o $(LIBOBJDIR)
	ar q $@ $(OBJS) $(LIBOBJDIR)/*.o
	@mv $(LIBA) ./lib
	@cp $(SRCDIR)/*.h ./inc/

clean:
	rm -rf ./lib/* ./inc/* $(OBJDIR) $(LIBOBJDIR) $(SRCDIR)/*.o
	
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
