# Target  name
TARGET = strtest 

# Directories
OBJDIR = obj
SRCDIR = src

# Libraries
INC += -I$(PROJBASE)/cm/foundation/inc -I$(PROJBASE)/cm/log/inc
# /usr/local/lib/liblog4cplus.a is included in libcm.a
LIBS = $(PROJBASE)/cm/lib/libcm.a

# Files and folders
SRCS := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.c $(SRCDIR)/*.cc)
OBJS := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:.cpp=.o))

include ../../cm/global.mk

all: $(TARGET)

# use static lib (*.a) by adding -lrt xxx.a in end of the line
$(TARGET): buildrepo $(OBJS)
	$(CXX) $(OBJS) -o $@ -lrt $(LIBS)
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
