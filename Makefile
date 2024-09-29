# Macros
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Rule to link object code files to create executable file
image_cropper: image_cropper.o bitmap.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rules to compile source code file to object code
image_cropper.o: image_cropper.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

bitmap.o: bitmap.cpp bitmap.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Cleanup rule
.PHONY: clean
clean:
	rm image_cropper *.o