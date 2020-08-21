BUILDDIR?=build

SRCDIR:=src
# SRC包含src文件中的所有文件名（包含路径）
SRC:=$(wildcard $(SRCDIR)/*.c)

CFI_INCLUDE?=/usr/include

# 关闭栈保护和NX Bit
CC:=clang
CFLAGS=-Wall -Wextra -Wtype-limits -Wsign-compare -DCFI_DEBUG -I$(CFI_INCLUDE) -I/usr/include/c++/8 -lbcm2835 -fno-stack-protector -z execstack -lpthread -fpermissive -lssl -lcrypto
LD:=$(CC)
LDFLAGS=$(CFLAGS)

# BASENAME删除了文件名中的路径，只保留文件名。basename取出文件扩展名之前的部分。
BASENAMES:=$(patsubst $(SRCDIR)/%,%,$(basename $(SRC)))
OFILES:=$(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC))
IRFILES:=$(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.bc,$(SRC))
# TARGET用于生成最终编译的目标文件，这里的模版是为所有的源文件均生成可执行文件，我们需要的是将目录下所有c文件链接生成一个最后的可执行文件
# TARGETS:=$(addprefix $(BUILDDIR)/,$(BASENAMES))
TARGETS:= $(BUILDDIR)/Smarthome-picon
MKDIR?=mkdir

# PICON Configeration
IGNORED_FUNCTIONS:=ignored_functions.txt
CFI_PASS?=/usr/lib/picon/libCFI.so
CFI_PRELINK?=/usr/bin/picon-prelink
OPT?=opt-3.7
OPTFLAGS:=-load $(CFI_PASS) -cfi -cfi-level=1 -cfi-ignore=$(IGNORED_FUNCTIONS)


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

$(TARGETS): $(_INJECTED)_all.o $(BUILDDIR)/all.o
	$(LD) -o $(TARGETS) $^ $(LDFLAGS)

clean:
	$(RM) -r $(BUILDDIR)
