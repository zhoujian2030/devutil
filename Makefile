ifeq ($(GTEST_SUPPORT), TRUE)

ifeq ($(DB_SUPPORT), TRUE)
SUBDIRS = cm \
	ipc \
	net \
	cpp \
	dao \
	ads \
	http \
	tel \
	sample \
	test 
else
SUBDIRS = cm \
	ipc \
	net \
	cpp \
	ads \
	http \
	tel \
	sample \
	test 
endif

else

ifeq ($(DB_SUPPORT), TRUE)
SUBDIRS = cm \
	ipc \
	net \
	cpp \
	dao \
	ads \
	http \
	sample
else
SUBDIRS = cm \
	ipc \
	net \
	cpp \
	ads \
	http \
	sample
endif

endif

BUILDDIRS = $(SUBDIRS:%=build-%)
INSTALLDIRS = $(SUBDIRS:%=install-%)
CLEANDIRS = $(SUBDIRS:%=clean-%)

# build target
all: $(BUILDDIRS)
$(SUBDIRS): $(BUILDDIRS)
$(BUILDDIRS):
	$(MAKE) -C $(@:build-%=%)

# clean target
clean: $(CLEANDIRS)
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

# specify the dependency here
ipc: cm
dao: cm
net: cm
ads: cm
test: ipc
test: net
http: net