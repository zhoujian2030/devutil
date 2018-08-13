all: $(OBJS)

clean:
	rm -rf $(LIBA) $(LIBDIR) $(OBJDIR) $(SRCDIR)/*.o $(DEPDIR)
	
install:
	# nothing to do

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
-include $(DEPDIR)/$(LIBNAME).d
endif
