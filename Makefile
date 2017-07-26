#Mandatory extra flags
CXXFLAGS := $(CXXFLAGS) -std=c++11 -O3 -DNDEBUG

#Configuration
INCLUDES := -Iinclude

.DEFAULT_GOAL := all
.PHONY: test

#Subdirectory fragments
MODULES_CLEAN := 
include tests/make.config
include samples/make.config

#Targets
all: $(TEST_TARGETS) $(SAMPLES)

clean:
	rm -f $(MODULES_CLEAN)

#Common rules
.cpp.o:
	$(CXX) -MD -c $< -o $@ $(INCLUDES) $(CXXFLAGS) $(CPPFLAGS)

#Handle dependency files
-include $(TEST_OBJS:.o=.d)
