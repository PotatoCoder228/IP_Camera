CXXFLAGS=-Os
LDFLAGS=-lm

all: our-script

our-script: our-script.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	-rm our-script *.o