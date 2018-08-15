LIBNAME = libcm
LIBDIR = lib
DEPDIR = .d

$(shell mkdir -p $(LIBDIR) >/dev/null)
$(shell mkdir -p $(DEPDIR) >/dev/null)

OBJS = log/obj/* foundation/obj/* thread/obj/* mq/obj/* util/obj/* 3pp/obj/* clog/obj/*
POST_BUILD=@sh ./postbuild.sh $(LIBNAME)
LIBA = $(LIBDIR)/$(LIBNAME).a

all: $(LIBA)

$(LIBA): $(OBJS)
	$(AR) rcs $@ $^
	@echo $(LIBNAME).a: $(OBJS) > $(DEPDIR)/$(LIBNAME).d
	$(POST_BUILD)

clean:
	rm -rf $(LIBDIR) $(DEPDIR)
	
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPDIR)/$(LIBNAME).d
endif