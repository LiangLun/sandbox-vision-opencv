CFLAGS = `pkg-config --cflags opencv libfreenect`
LIBS = `pkg-config --libs opencv`

% : %.cpp
	g++  $(CFLAGS) -o $@ $< $(LIBS) -lfreenect_cv -lm
 

