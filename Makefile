SUBDIRS = cm \
	log \
	mq \
	ipc \
	net \
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
net: cm
test: ipc
