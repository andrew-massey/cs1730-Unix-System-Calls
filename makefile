all: calcpp datecpp headcpp lncpp lscpp mkdircpp tailcpp truecpp falsecpp wccpp pwdcpp cksumcpp
	

calcpp: cal.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors cal.cpp
	g++ -o cal cal.o

datecpp: date.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors date.cpp
	g++ -o date date.o
   
headcpp: head.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors head.cpp
	g++ -o head head.o
    
lncpp: ln.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors ln.cpp
	g++ -o ln ln.o

lscpp: ls.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors ls.cpp
	g++ -o ls ls.o

mkdircpp: mkdir.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors mkdir.cpp
	g++ -o mkdir mkdir.o

envcpp: env.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors env.cpp
	g++ -o env env.o

tailcpp: tail.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors tail.cpp
	g++ -o tail tail.o

wccpp: wc.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors wc.cpp
	g++ -o wc wc.o

pwdcpp: pwd.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors pwd.cpp
	g++ -o pwd pwd.o
 
truecpp: true.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors true.cpp
	g++ -o true true.o
 
falsecpp: false.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors false.cpp
	g++ -o false false.o
 
cksumcpp: cksum.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors cksum.cpp
 
clean:
	rm -f *.o
	rm -f ./cal
	rm -f ./date
	rm -f ./head
	rm -f ./ln
	rm -f ./ls
	rm -f ./mkdir
	rm -f ./env
	rm -f ./tail
	rm -f ./true
	rm -f ./false
	rm -f ./wc
	rm -f ./pwd
	rm -f ./cksum