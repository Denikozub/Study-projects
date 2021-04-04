ASSEMBLER= wine ml
ASSEMBLER_FLAGS= /c /coff /Fl /I"C:\masm\include"
LINKER= wine link
LINKER_FLAGS= /subsystem:console /LIBPATH:"C:\masm\lib"
PASCAL= wine fpc

SOURCES= module_asm.asm
PROGRAM= program.exe

SOURCES_UTF= $(patsubst %.asm, %.utf8.asm,  $(SOURCES))
OBJS=$(patsubst %.asm, %.obj, $(SOURCES))
LISTINGS=$(patsubst %.asm, %.lst, $(SOURCES))


program.exe: main.pas $(OBJS)
	$(PASCAL) -o$@ $<

%.obj: %.asm
	$(ASSEMBLER) $(ASSEMBLER_FLAGS) $^

%.utf8.asm: %.asm
	iconv -f cp1251 -t utf8 < $^ > $@

convert_to_utf: $(SOURCES_UTF)

clean:
	rm -f $(OBJS) $(LISTINGS) $(PROGRAM) $(SOURCES_UTF)

run:
	wine ./$(PROGRAM)

