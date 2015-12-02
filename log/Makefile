# Target  name
TARGET=log
LIBA = liblog.a

# Directories
OBJDIR = obj
SRCDIR = src

# Libraries
INC += -I$(PROJBASE)/cm/inc
LIBS = /usr/local/lib/liblog4cplus.a

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include ../cm/global.mk

all: $(LIBA) $(TARGET)

# use static lib (*.a) by adding -lrt xxx.a in end of the line
$(TARGET): 
	$(CXX) $(OBJS) -o $@ -lrt $(LIBS)
	@mv $(TARGET) $(OBJDIR)

# Targets
$(LIBA): buildrepo $(OBJS)
	ar q $@ $(OBJS) 
	@mv $(LIBA) ./lib
	@cp $(SRCDIR)/*.h ./inc/

clean:
	rm -rf ./lib/* $(OBJDIR) $(SRCDIR)/*.o
	
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
