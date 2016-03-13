SUBDIRS = cm \
	log \
	ipc \
	cpp \
	dao \
	test 

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	make -C $@

# specify the dependency here
ipc: cm
dao: cm
test: ipc
