##### Compiler flags #####

CC=gcc
CFLAGS=-s -Os -flto

##### ASM initialising #####


SRC_ASM=src_asm
BUILD_ASM=build_asm

ASM_SOURCE_FILES=${SRC_ASM}/print.c ${SRC_ASM}/bin.c ${SRC_ASM}/stdstr.c \
${SRC_ASM}/objdump_x2017.c ${SRC_ASM}/parser.c ${SRC_ASM}/file.c

ASM_OBJ_FILES=${patsubst ${SRC_ASM}/%.c, ${BUILD_ASM}/%.o, ${ASM_SOURCE_FILES}}


##### VM initialising #####


SRC_VM=src_vm
BUILD_VM=build_vm

PARSER_OBJ_FILES=${BUILD_ASM}/bin.o ${BUILD_ASM}/stdstr.o ${BUILD_ASM}/file.o \
${BUILD_ASM}/parser.o

VM_SOURCE_FILES=${SRC_VM}/vm_x2017.c  ${SRC_VM}/operations.c ${SRC_VM}/stack.c \
${SRC_VM}/getters.c ${SRC_VM}/execute.c

VM_OBJ_FILES=${patsubst ${SRC_VM}/%.c, ${BUILD_VM}/%.o, ${VM_SOURCE_FILES}}

LIB=lib


##### Recipe Targets #####


TARGET_ASM=objdump_x2017
TARGET_VM=vm_x2017

all : ${TARGET_ASM} ${TARGET_VM}


##### VM Building #####


${TARGET_VM} : ${VM_OBJ_FILES} ${PARSER_OBJ_FILES}
	@echo "### Building $@ ###"
	${CC} ${CFLAGS} -I${LIB} -o $@ $^
	@echo "⭐⭐⭐ ${TARGET_VM} successfully built! ⭐⭐⭐"

${BUILD_VM}/%.o : ${SRC_VM}/%.c | ${BUILD_VM}
	@echo "### Building $@ file ###"
	${CC} ${CFLAGS} -I${LIB} -c -o $@ $<

${BUILD_VM} :
	@echo "### Creating Build Directory ###"
	mkdir ${BUILD_VM}


##### ASM Building #####


${TARGET_ASM} : ${ASM_OBJ_FILES}
	@echo "### Building $@ ###"	
	${CC} ${CFLAGS} -I${LIB} -o $@ $^
	@echo "⭐⭐⭐ ${TARGET_ASM} successfully built! ⭐⭐⭐"	

${BUILD_ASM}/%.o : ${SRC_ASM}/%.c | ${BUILD_ASM}
	@echo "### Building $@ file ###"
	${CC} ${CFLAGS} -I${LIB} -c -o $@ $<
	
${BUILD_ASM} :
	@echo "### Creating Build Directory ###"
	mkdir ${BUILD_ASM}


.PHONY : clean
.PHONY : tests
.PHONY : run_tests

clean :
	rm -f ${TARGET_ASM}
	rm -rf ${BUILD_ASM}
	rm -f ${TARGET_VM}
	rm -rf ${BUILD_VM}
	
tests :
	@echo "Tests have already been built prior to submission. Do make run_tests to run them."

run_tests : ${TARGET_ASM} ${TARGET_VM}
	bash test.sh

