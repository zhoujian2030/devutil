# Target  name
LIBA = libnet.a

# Directories
OBJDIR = obj
SRCDIR = src
INCDIR = inc
LIBDIR = lib

# Libraries
INC += -I$(PROJBASE)/cm/inc -I$(PROJBASE)/log/inc
LIBS += $(PROJBASE)/log/lib/liblog.a

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

DEPOBJS := $(PROJBASE)/cm/obj/*.o

include ../cm/global.mk

all: $(LIBA)

$(LIBA): buildrepo $(OBJS)
	ar q $@ $(OBJS)
	@mkdir -p $(LIBDIR)
	@mv $(LIBA) ./lib
	@mkdir -p $(INCDIR)
	@cp $(SRCDIR)/*.h ./inc/

clean:
	rm -rf $(LIBDIR) $(INCDIR) $(OBJDIR) $(SRCDIR)/*.o
	
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
