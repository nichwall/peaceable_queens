PNAME = PeaceableQueens

# Directory structure
SDIR = src
ODIR = obj
BDIR = bin

# Compiler option
INC = $(SDIR)
INC_PARAMS = $(foreach d, $(INC), -I$d)
CPPFLAGS = -std=c++11 -g
LINKFLAGS = 

# Compilation commands
CPPC = g++ $(CPPFLAGS) $(INC_PARAMS) $(LINKFLAGS)
CPPL = g++ $(CPPFLAGS) $(LINKFLAGS)

#Default make target: setup the environment, then build the program
.PHONY: all
all: | toolchain $(PNAME)

SRCS = main.cpp puzzle.cpp
SOBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRCS))

$(BDIR)/$(PNAME): $(SOBJ)
	$(CPPL) -o $@ $^

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CPPC) -c -o $@ $^

%.o: %.cpp
	$(CPPC) -c -o $@ $<

#main.o: func.hpp
#func.o: func.hpp

.PHONY: clean
clean:
	rm -rf $(ODIR) $(BDIR)

.PHONY: toolchain pmain $(PNAME)
toolchain: | $(BDIR) $(ODIR)
pmain: $(BDIR)/$(PNAME)
$(PNAME): | toolchain pmain

$(ODIR):
	@mkdir $(ODIR)

$(BDIR):
	@mkdir $(BDIR)
