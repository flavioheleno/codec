COMPILER = gcc
CFLAGS = -Wall -O2 -lm -ansi -pedantic
NAME = f3

all:
	$(COMPILER) $(CFLAGS) -c *.c
	$(COMPILER) $(CFLAGS) -o $(NAME) *.o

clean:
	rm -rf *.o $(NAME) $(NAME).tar.gz

run: all
	./$(NAME)

encode8: all
	./$(NAME) --encode img/8x8.bmp encoded.fhb

encode16: all
	./$(NAME) --encode img/16x16.bmp encoded.fhb

encode640: all
	./$(NAME) --encode img/640x400.bmp encoded.fhb

encode1280: all
	./$(NAME) --encode img/1280x800.bmp encoded.fhb

encodelena: all
	./$(NAME) --encode img/lena.bmp encoded.fhb

decode: all
	./$(NAME) --decode encoded.fhb probe.bmp

targz:
	tar -cf $(NAME).tar *.h *.c img/
	gzip -f $(NAME).tar
