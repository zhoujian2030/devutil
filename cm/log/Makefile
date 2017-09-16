# Target  name
TARGET=
LIBA = 

# Directories
OBJDIR = obj
SRCDIR = src
INCDIR = inc
LIBDIR = lib

# Libraries
INC += 
ifeq ($(PLATFORM), arm)
LIBS = /usr/local/log4cplusarm/lib/liblog4cplus.a
else
LIBS = /usr/local/log4cplusx86/lib/liblog4cplus.a
endif

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include ../global.mk

# Targets
all: buildrepo $(OBJS)
	$(AR) x $(LIBS) 
	@mv *.o $(OBJDIR)
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
