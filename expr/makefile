CC = gcc
CFLAGS = -g -Wall
OBJDIR = ./obj
VM = $(OBJDIR)/vm.o
LEX = $(OBJDIR)/lex.o
MAIN = $(OBJDIR)/main.o
DESC = $(OBJDIR)/parse_ll_rec_desc.o
ASC = $(OBJDIR)/parse_lr_rec_asc.o
COMMON = $(VM) $(LEX) $(MAIN)

$(VM): vm.c vm.h
	$(CC) $< -c -o $@ $(CFLAGS)

$(LEX): lex.c lex.h
	$(CC) $< -c -o $@ $(CFLAGS)

$(MAIN): main.c
	$(CC) $< -c -o $@ $(CFLAGS)
		
$(DESC): parse_ll_rec_desc.c parse.h
	$(CC) $< -c -o $@ $(CFLAGS)

$(ASC): parse_lr_rec_asc.c parse.h
	$(CC) $< -c -o $@ $(CFLAGS)

desc: $(COMMON) $(DESC)
	$(CC) $? -o $@.bin $(CFLAGS)

asc: $(COMMON) $(ASC)
	$(CC) $? -o $@.bin $(CFLAGS)

all: desc asc

clean:
	rm $(OBJDIR)/*.o
	rm *.bin
	
