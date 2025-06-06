ifdef CROSS_COMPILER
CC = $(CROSS_COMPILER)
else
ifndef CC
# default compiler
CC = gcc
endif
endif

ifndef COMPAT_FLAGS
# Should be replaced by proper warning options
#COMPAT_FLAGS = -std=c89 -pedantic -Wpedantic
COMPAT_FLAGS =
endif

ifndef OPT_FLAGS
OPT_FLAGS = -O2
endif

ifndef WARN_FLAGS
WARN_FLAGS = -Wall -Wextra -Wconversion -Wsign-compare -Wdeclaration-after-statement -Wunused-function
endif

ifndef WERROR
WERROR = -Werror
endif

CPPFLAGS = $(EXTRA_CPPFLAGS) -Isljit_src
CFLAGS += $(COMPAT_FLAGS) $(OPT_FLAGS) $(WARN_FLAGS) $(WERROR)
REGEX_CFLAGS += $(CFLAGS) -fshort-wchar
EXAMPLE_CFLAGS += $(CFLAGS) -Wno-unused-but-set-variable
LDFLAGS = $(EXTRA_LDFLAGS)

BINDIR = bin
SRCDIR = sljit_src
TESTDIR = test_src
REGEXDIR = regex_src
EXAMPLEDIR = doc/tutorial

TARGET = $(BINDIR)/sljit_test $(BINDIR)/regex_test
EXAMPLE_TARGET = $(BINDIR)/func_call $(BINDIR)/first_program $(BINDIR)/branch $(BINDIR)/loop $(BINDIR)/array_access $(BINDIR)/func_call $(BINDIR)/struct_access $(BINDIR)/temp_var $(BINDIR)/brainfuck

SLJIT_HEADERS = $(SRCDIR)/sljitLir.h $(SRCDIR)/sljitConfig.h $(SRCDIR)/sljitConfigInternal.h

SLJIT_LIR_FILES = $(SRCDIR)/sljitLir.c $(SRCDIR)/sljitUtils.c \
	$(SRCDIR)/allocator_src/sljitExecAllocatorCore.c $(SRCDIR)/allocator_src/sljitExecAllocatorApple.c \
	$(SRCDIR)/allocator_src/sljitExecAllocatorPosix.c $(SRCDIR)/allocator_src/sljitExecAllocatorWindows.c \
	$(SRCDIR)/allocator_src/sljitProtExecAllocatorNetBSD.c $(SRCDIR)/allocator_src/sljitProtExecAllocatorPosix.c \
	$(SRCDIR)/allocator_src/sljitWXExecAllocatorPosix.c $(SRCDIR)/allocator_src/sljitWXExecAllocatorWindows.c \
	$(SRCDIR)/sljitNativeARM_32.c $(SRCDIR)/sljitNativeARM_T2_32.c $(SRCDIR)/sljitNativeARM_64.c \
	$(SRCDIR)/sljitNativeMIPS_common.c $(SRCDIR)/sljitNativeMIPS_32.c $(SRCDIR)/sljitNativeMIPS_64.c \
	$(SRCDIR)/sljitNativePPC_common.c $(SRCDIR)/sljitNativePPC_32.c $(SRCDIR)/sljitNativePPC_64.c \
	$(SRCDIR)/sljitNativeRISCV_common.c $(SRCDIR)/sljitNativeRISCV_32.c $(SRCDIR)/sljitNativeRISCV_64.c \
	$(SRCDIR)/sljitNativeS390X.c $(SRCDIR)/sljitNativeLOONGARCH_64.c \
	$(SRCDIR)/sljitNativeX86_common.c $(SRCDIR)/sljitNativeX86_32.c $(SRCDIR)/sljitNativeX86_64.c

.PHONY: all clean examples

all: $(TARGET)

clean:
	-$(RM) $(BINDIR)/*.o $(BINDIR)/sljit_test $(BINDIR)/regex_test $(EXAMPLE_TARGET)

$(BINDIR)/.keep :
	mkdir -p $(BINDIR)
	@touch $@

$(BINDIR)/sljitLir.o : $(BINDIR)/.keep $(SLJIT_LIR_FILES) $(SLJIT_HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $(SRCDIR)/sljitLir.c

$(BINDIR)/sljitMain.o : $(TESTDIR)/sljitMain.c $(BINDIR)/.keep $(SLJIT_HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $(TESTDIR)/sljitMain.c

$(BINDIR)/regexMain.o : $(REGEXDIR)/regexMain.c $(BINDIR)/.keep $(SLJIT_HEADERS)
	$(CC) $(CPPFLAGS) $(REGEX_CFLAGS) -c -o $@ $(REGEXDIR)/regexMain.c

$(BINDIR)/regexJIT.o : $(REGEXDIR)/regexJIT.c $(BINDIR)/.keep $(SLJIT_HEADERS) $(REGEXDIR)/regexJIT.h
	$(CC) $(CPPFLAGS) $(REGEX_CFLAGS) -c -o $@ $(REGEXDIR)/regexJIT.c

$(BINDIR)/sljit_test: $(BINDIR)/.keep $(BINDIR)/sljitMain.o $(TESTDIR)/sljitTest.c $(SRCDIR)/sljitLir.c $(SLJIT_LIR_FILES) $(SLJIT_HEADERS) $(TESTDIR)/sljitConfigPre.h $(TESTDIR)/sljitConfigPost.h $(TESTDIR)/sljitTestCall.h $(TESTDIR)/sljitTestFloat.h $(TESTDIR)/sljitTestSimd.h
	$(CC) $(CPPFLAGS) -DSLJIT_HAVE_CONFIG_PRE=1 -I$(TESTDIR) $(CFLAGS) $(LDFLAGS) $(BINDIR)/sljitMain.o $(TESTDIR)/sljitTest.c $(SRCDIR)/sljitLir.c -o $@ -lm -lpthread $(EXTRA_LIBS)

$(BINDIR)/regex_test: $(BINDIR)/.keep $(BINDIR)/regexMain.o $(BINDIR)/regexJIT.o $(BINDIR)/sljitLir.o
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(BINDIR)/regexMain.o $(BINDIR)/regexJIT.o $(BINDIR)/sljitLir.o -o $@ -lm -lpthread $(EXTRA_LIBS)

examples: $(EXAMPLE_TARGET)

$(BINDIR)/first_program: $(EXAMPLEDIR)/first_program.c $(BINDIR)/.keep $(BINDIR)/sljitLir.o
	$(CC) $(CPPFLAGS) $(EXAMPLE_CFLAGS) $(LDFLAGS) $(EXAMPLEDIR)/first_program.c $(BINDIR)/sljitLir.o -o $@ -lm -lpthread $(EXTRA_LIBS)

$(BINDIR)/branch: $(EXAMPLEDIR)/branch.c $(BINDIR)/.keep $(BINDIR)/sljitLir.o
	$(CC) $(CPPFLAGS) $(EXAMPLE_CFLAGS) $(LDFLAGS) $(EXAMPLEDIR)/branch.c $(BINDIR)/sljitLir.o -o $@ -lm -lpthread $(EXTRA_LIBS)

$(BINDIR)/loop: $(EXAMPLEDIR)/loop.c $(BINDIR)/.keep $(BINDIR)/sljitLir.o
	$(CC) $(CPPFLAGS) $(EXAMPLE_CFLAGS) $(LDFLAGS) $(EXAMPLEDIR)/loop.c $(BINDIR)/sljitLir.o -o $@ -lm -lpthread $(EXTRA_LIBS)

$(BINDIR)/array_access: $(EXAMPLEDIR)/array_access.c $(BINDIR)/.keep $(BINDIR)/sljitLir.o
	$(CC) $(CPPFLAGS) $(EXAMPLE_CFLAGS) $(LDFLAGS) $(EXAMPLEDIR)/array_access.c $(BINDIR)/sljitLir.o -o $@ -lm -lpthread $(EXTRA_LIBS)

$(BINDIR)/func_call: $(EXAMPLEDIR)/func_call.c $(BINDIR)/.keep $(BINDIR)/sljitLir.o
	$(CC) $(CPPFLAGS) $(EXAMPLE_CFLAGS) $(LDFLAGS) $(EXAMPLEDIR)/func_call.c $(BINDIR)/sljitLir.o -o $@ -lm -lpthread $(EXTRA_LIBS)

$(BINDIR)/struct_access: $(EXAMPLEDIR)/struct_access.c $(BINDIR)/.keep $(BINDIR)/sljitLir.o
	$(CC) $(CPPFLAGS) $(EXAMPLE_CFLAGS) $(LDFLAGS) $(EXAMPLEDIR)/struct_access.c $(BINDIR)/sljitLir.o -o $@ -lm -lpthread $(EXTRA_LIBS)

$(BINDIR)/temp_var: $(EXAMPLEDIR)/temp_var.c $(BINDIR)/.keep $(BINDIR)/sljitLir.o
	$(CC) $(CPPFLAGS) $(EXAMPLE_CFLAGS) $(LDFLAGS) $(EXAMPLEDIR)/temp_var.c $(BINDIR)/sljitLir.o -o $@ -lm -lpthread $(EXTRA_LIBS)

$(BINDIR)/brainfuck: $(EXAMPLEDIR)/brainfuck.c $(BINDIR)/.keep $(BINDIR)/sljitLir.o
	$(CC) $(CPPFLAGS) $(EXAMPLE_CFLAGS) $(LDFLAGS) $(EXAMPLEDIR)/brainfuck.c $(BINDIR)/sljitLir.o -o $@ -lm -lpthread $(EXTRA_LIBS)
