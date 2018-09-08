CC = gcc
CFLAGS = -g -Wall
OBJDIR = ./obj
VM = $(OBJDIR)/vm.o
LEX = $(OBJDIR)/lex.o
MAIN = $(OBJDIR)/main.o
DESC = $(OBJDIR)/parse_ll_rec_desc.o
DESC_OPT = $(OBJDIR)/parse_ll_rec_desc_opt.o
ASC = $(OBJDIR)/parse_lr_rec_asc.o
LRPDA = $(OBJDIR)/parse_lr_pda.o
COMMON = $(VM) $(LEX) $(MAIN)

$(VM): vm.c vm.h
	$(CC) $< -c -o $@ $(CFLAGS)

$(LEX): lex.c lex.h
	$(CC) $< -c -o $@ $(CFLAGS)

$(MAIN): main.c
	$(CC) $< -c -o $@ $(CFLAGS)
		
$(DESC): parse_ll_rec_desc.c parse.h
	$(CC) $< -c -o $@ $(CFLAGS)

$(DESC_OPT): parse_ll_rec_desc_opt.c parse.h
	$(CC) $< -c -o $@ $(CFLAGS)
	
$(ASC): parse_lr_rec_asc.c parse.h
	$(CC) $< -c -o $@ $(CFLAGS)

$(LRPDA): parse_lr_pda.c parse.h
	$(CC) $< -c -o $@ $(CFLAGS)

lrpda: $(COMMON) $(LRPDA)
	$(CC) $? -o $@.bin $(CFLAGS)

desc_opt: $(COMMON) $(DESC_OPT)
	$(CC) $? -o $@.bin $(CFLAGS)

desc: $(COMMON) $(DESC)
	$(CC) $? -o $@.bin $(CFLAGS)

asc: $(COMMON) $(ASC)
	$(CC) $? -o $@.bin $(CFLAGS)

all: desc desc_opt asc lrpda

clean:
	rm $(OBJDIR)/*.o
	rm *.bin
