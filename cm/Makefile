# LIBNAME = libcm
# LIBDIR = lib
# DEPDIR = .d

SUBDIRS = log \
	foundation \
	thread \
	mq \
	util

BUILDDIRS = $(SUBDIRS:%=build-%)
INSTALLDIRS = $(SUBDIRS:%=install-%)
CLEANDIRS = $(SUBDIRS:%=clean-%)

POSTBUILD := 1
POSTCLEAN := 2

# build target
all: $(BUILDDIRS) $(POSTBUILD)
$(SUBDIRS): $(BUILDDIRS)
$(BUILDDIRS):
	$(MAKE) -C $(@:build-%=%)

# clean target
clean: $(CLEANDIRS) $(POSTCLEAN)
$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean

# install target
install: $(INSTALLDIRS) all
$(INSTALLDIRS):
	$(MAKE) -C $(@:install-%=%) install

.PHONY: subdirs $(SUBDIRS)
.PHONY: subdirs $(BUILDDIRS)
.PHONY: subdirs $(INSTALLDIRS)
.PHONY: subdirs $(CLEANDIRS)

.PHONY: all install clean

.PHONY: $(POSTBUILD)
$(POSTBUILD): 
	$(MAKE) -f build_libcm.mk

.PHONY: $(POSTCLEAN)
$(POSTCLEAN):
	$(MAKE) -f build_libcm.mk clean

# specify the dependency here
thread: log
foundation: log