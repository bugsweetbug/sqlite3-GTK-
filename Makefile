src 	= $(wildcard *.c)


flags 	+= `pkg-config --cflags --libs gtk+-2.0`
#flags 	+= -Wall
flags 	+= -lpthread -ldl
#flags 	+= -g
out = main
$(out):$(src)
	gcc -o $(out) $^ $(flags)
.PHONY:clean ALL
clean:
	@rm -rf $(out)
