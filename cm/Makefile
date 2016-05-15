LIBA = libcm.a
LIBDIR = lib

SUBDIRS = foundation \
	thread \
	mq

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

OBJS = foundation/obj/* thread/obj/* mq/obj/*

.PHONY: $(POSTBUILD)
$(POSTBUILD): 
	ar q $(LIBA) $(OBJS)
	@mkdir -p $(LIBDIR)
	@mv $(LIBA) $(LIBDIR)

.PHONY: $(POSTCLEAN)
$(POSTCLEAN):
	rm -rf $(LIBDIR)


# specify the dependency here
