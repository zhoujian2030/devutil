# Complier
CC = gcc
CXX = g++
CFLAGS = -Wall -g -O2 -DNDEBUG -fPIC 

.%.d: %.cpp
	$(CXX) $(INC) -MM $< > $@
	@$(CXX) $(INC) -MM $< | sed s/"^"/"\."/  |  sed s/"^\. "/" "/  | \
                sed s/"\.o"/"\.d"/  >> $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(INC) $(CFLAGS) -c $< -o $@

.%.d: %.c
	$(CC) $(INC) -MM $< > $@
	@$(CC) $(INC) -MM $< | sed s/"^"/"\."/  |  sed s/"^\. "/" "/  | \
                sed s/"\.o"/"\.d"/  >> $@
%.o: %.c
	$(CC) $(INC) $(CFLAGS)  -c $<
	
%.o: %.cc
	$(CXX) $(INC) $(CFLAGS)  -c $<
