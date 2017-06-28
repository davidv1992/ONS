#Mandatory extra flags
CXXFLAGS := $(CXXFLAGS) -std=c++11 -O3 -DNDEBUG

#Configuration
INCLUDES := -Iinclude

TEST_OBJS := test.o

.DEFAULT_GOAL := all

#Subdirectory fragments
MODULES_CLEAN := 
include tests/make.config

#Targets
all: test $(TEST_TARGETS)

test: $(TEST_OBJS)
	g++ $(CXXFLAGS) $(LDFLAGS) -o test $(TEST_OBJS)

clean:
	rm -f $(TEST_OBJS)
	rm -f $(TEST_OBJS:.o=.d)
	rm -f $(MODULES_CLEAN)
	rm -f test

#Common rules
.cpp.o:
	$(CXX) -MD -c $< -o $@ $(INCLUDES) $(CXXFLAGS) $(CPPFLAGS)

#Handle dependency files
-include $(TEST_OBJS:.o=.d)
