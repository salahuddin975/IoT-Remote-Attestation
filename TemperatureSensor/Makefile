BUILDDIR?=build

SRCDIR:=src
SRC:=$(wildcard $(SRCDIR)/*.c)

CFI_INCLUDE?=/usr/include

CC:=clang
CFLAGS=-pedantic -Wall -Wextra -Wtype-limits -Wsign-compare
LD:=$(CC)
LDFLAGS=$(CFLAGS)

BASENAMES:=$(patsubst $(SRCDIR)/%,%,$(basename $(SRC)))
OFILES:=$(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC))
IRFILES:=$(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.bc,$(SRC))
TARGETS:= $(BUILDDIR)/temp_cfi
MKDIR?=mkdir

# PICON Configeration
IGNORED_FUNCTIONS:=ignored_functions.txt
CFI_PASS?=/usr/lib/picon/libCFI.so
CFI_PRELINK?=/usr/bin/picon-prelink
OPT?=opt-3.7
OPTFLAGS:=-load $(CFI_PASS) -cfi -cfi-level=0 -cfi-ignore=$(IGNORED_FUNCTIONS)


######################
_INJECTED:=$(BUILDDIR)/cfi_injected
_INJECTED_OFILES:=$(patsubst $(SRCDIR)/%.c,$(_INJECTED)_%.o,$(SRC))


.NOTPARALLEL: all clean
.PHONY: all clean

all: clean $(TARGETS)

$(BUILDDIR):
	$(MKDIR) -p $@

$(BUILDDIR)/%.bc: $(SRCDIR)/%.c $(BUILDDIR)
	$(CC) $(CFLAGS) -emit-llvm -o $@ -c $<

$(BUILDDIR)/all.bc: $(IRFILES)
	llvm-link $(LL_FLAGS) -o $@ $^

$(BUILDDIR)/all_cfi.bc: $(BUILDDIR)/all.bc $(IGNORED_FUNCTIONS)
	$(OPT) $(OPTFLAGS) -cfi-prefix=$(basename $@) -S -o $@ $<

$(BUILDDIR)/all.o: $(BUILDDIR)/all_cfi.bc
	$(CC) $(CFLAGS) -o $@ -c $<

$(_INJECTED)_all.c: $(BUILDDIR)/all_cfi.bc
	$(CFI_PRELINK) $(basename $<) > $@

$(_INJECTED)_all.o: $(_INJECTED)_all.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILDDIR)/libmyifttt.a: $(BUILDDIR)/all.o
	ar -rcs $@ $<

$(TARGETS): $(_INJECTED)_all.o $(BUILDDIR)/all.o $(BUILDDIR)/libmyifttt.a
	$(LD) -o $(TARGETS) $^ $(LDFLAGS) -L. -lm -lwiringPi  -lmyifttt -lcurl 

clean:
	$(RM) -r $(BUILDDIR)
