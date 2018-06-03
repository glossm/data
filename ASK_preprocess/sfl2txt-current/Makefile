CPPFLAGS=-g
CXX=g++
#CXX=i586-mingw32msvc-c++
SRC=src
DOC=doc


all: ${SRC}/converter.h ${SRC}/read_sfl.o ${SRC}/write_sfl.o ${SRC}/dump.o ${SRC}/import.o ${SRC}/export.o ${SRC}/settings.o ${SRC}/converter.o ${DOC}/sfl2txt.1.gz
	${CXX} ${CPPFLAGS} ${SRC}/*.o -o sfl2txt

install:
	cp sfl2txt /usr/bin/
	cp ${DOC}/sfl2txt.1.gz /usr/share/man/man1/
	mkdir -p /usr/share/doc/sfl2txt
	cp sfl2txt.sample.conf /usr/share/doc/sfl2txt
	cp CHANGELOG /usr/share/doc/sfl2txt
	cp COPYING /usr/share/doc/sfl2txt
	cp README /usr/share/doc/sfl2txt

uninstall:
	rm /usr/bin/sfl2txt
	rm /usr/share/man/man1/sfl2txt.1.gz
	rm -rf /usr/share/doc/sfl2txt/
	
clean:
	rm -f ${SRC}/*.o ${SRC}/*.*~ ${SRC}/*~ ${DOC}/*~ ./*~ *.gch ${DOC}/sfl2txt.1.gz sfl2txt

${SRC}/read_sfl.o: ${SRC}/read_sfl.cpp ${SRC}/converter.h

${SRC}/write_sfl.o: ${SRC}/write_sfl.cpp ${SRC}/converter.h

${SRC}/dump.o: ${SRC}/dump.cpp ${SRC}/converter.h

${SRC}/import.o: ${SRC}/import.cpp ${SRC}/converter.h

${SRC}/export.o: ${SRC}/export.cpp ${SRC}/converter.h

${SRC}/settings.o: ${SRC}/settings.cpp ${SRC}/converter.h

${SRC}/converter.o: ${SRC}/converter.cpp ${SRC}/converter.h

${DOC}/sfl2txt.1.gz: ${DOC}/sfl2txt.man
	cp ${DOC}/sfl2txt.man ${DOC}/sfl2txt.1 && gzip ${DOC}/sfl2txt.1


