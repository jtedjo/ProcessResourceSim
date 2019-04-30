cc=g++
LIB=lib
SDIR=src
BDIR=bin
FILE1=$(SDIR)/Scheduler.cpp
FILE2=$(SDIR)/Resource.cpp

main: $(SDIR)/Main.cpp $(FILE1)
	mkdir -p $(BDIR)
	$(cc) -o $(BDIR)/project1 $(SDIR)/Main.cpp $(FILE1) $(FILE2)
run:
	$(BDIR)/project1 < $(LIB)/input.txt	
clean:
	rm -rf $(BDIR)
