# MCOLR_751
# see LICENSE file for copyright and license details.

include config.mk

SRC = main.c

all: mcolr

options:
	@echo MCOLR_751 build options:
	@echo "FLAGS    = ${FLAGS}"
	@echo "LIBS     = ${LIBS}"
	@echo "CC       = ${CC}"
	@echo "Version  = ${VERSION}"

mcolr: options clean
	${CC} -o MCOLR-751dot${VERSION} ${SRC} ${FLAGS} ${LIBS}
	sudo chmod 755 MCOLR-751dot${VERSION}

run: mcolr
	printf "\n\n\n"
	./MCOLR-751dot${VERSION}

clean:
	rm -f MCOLR-751dot*

clear:
	clear
