all: countPass.so

LLVM_CONFIG = llvm-config
LLVM_CXXFLAGS = $(shell $(LLVM_CONFIG) --cxxflags)
LLVM_LDFLAGS = $(shell $(LLVM_CONFIG) --ldflags)

countPass.o: countPass.cpp
	clang++ -c countPass.cpp $(LLVM_CXXFLAGS)

countPass.so: countPass.o
	clang++ -shared -o countPass.so countPass.o $(LLVM_LDFLAGS)

clean:
	rm -f countPass.o test.ll

count: countPass.so test.ll
	opt -load ./countPass.so -CountPass -enable-new-pm=0 test.ll

countTest: countPass.so test.ll
	opt -load ./countPass.so --help | grep CountPass

test.ll: test.txt
	opt --mem2reg test.txt -S > test.ll


