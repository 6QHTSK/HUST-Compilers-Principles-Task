all: minic

OBJS =  token.o  \
		main.o \

token.cpp: minic.l
	flex -o $@ minic.l

%.o: %.cpp
	g++  -o $@ $< -lpthread -ldl -c 

minic: $(OBJS)
	g++ -o $@ $(OBJS) -lfl -lstdc++  -lpthread -ldl

clean:
	$(RM) -rf $(OBJS) minic