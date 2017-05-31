#Mandatory extra flags
CXXFLAGS := $(CXXFLAGS) -std=c++11 -O3

#Configuration
INCLUDES := -Iinclude

TEST_OBJS := test.o

#Targets
test: $(TEST_OBJS)
	g++ $(CXXFLAGS) $(LDFLAGS) -o test $(TEST_OBJS)

clean:
	rm -f $(TEST_OBJS)
	rm -f $(TEST_OBJS:.o=.d)
	rm -f test

#Common rules
.cpp.o:
	$(CXX) -MD -c $< -o $@ $(INCLUDES) $(CXXFLAGS) $(CPPFLAGS)

#Handle dependency files
-include $(TEST_OBJS:.o=.d)
