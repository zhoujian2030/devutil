# Target  name
TARGET=
LIBA = liblog.a

# Directories
OBJDIR = obj
SRCDIR = src
INCDIR = inc
LIBDIR = lib

# Libraries
INC += 
LIBS = /usr/local/lib/liblog4cplus.a

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include ../global.mk

all: $(LIBA)

# Targets
$(LIBA): buildrepo $(OBJS)
	ar x $(LIBS) 
	@mv *.o $(OBJDIR)
	ar q $@ $(OBJDIR)/*.o
	@mkdir -p $(LIBDIR)
	@mv $(LIBA) $(LIBDIR)
	@mkdir -p $(INCDIR)
	@cp $(SRCDIR)/*.h $(INCDIR)

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
