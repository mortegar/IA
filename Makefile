flags = -Wno-deprecated -g -ggdb -lm
semilla = 2

AG:ag.c
	gcc ag.c -o AG $(flags)
exe:AG
	./AG
clean: 
	rm -rf AG
	rm -rf galog.txt
	rm -rf *~
