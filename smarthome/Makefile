BUILDDIR?=build

SRCDIR:=src
# SRC包含src文件中的所有文件名（包含路径）
SRC:=$(wildcard $(SRCDIR)/*.c)

CFI_INCLUDE?=/usr/include

# 关闭栈保护和NX Bit
CC:=clang
CFLAGS+=-I$(CFI_INCLUDE) -I/usr/include/c++/8 -I/usr/include/arm-linux-gnueabihf/c++/8/ -L /usr/lib/gcc/arm-linux-gnueabihf/8/ -Lbcm2835
CFLAGS+=-Wall -Wextra -Wtype-limits -Wsign-compare -fno-stack-protector -lpthread -lssl -lcrypto -z execstack -DCFI_DEBUG
LD:=$(CC)
LDFLAGS:=$(CFLAGS)

# BASENAME删除了文件名中的路径，只保留文件名。basename取出文件扩展名之前的部分。
BASENAMES:=$(patsubst $(SRCDIR)/%,%,$(basename $(SRC)))
OFILES:=$(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC))
# TARGET用于生成最终编译的目标文件，这里的模版是为所有的源文件均生成可执行文件，我们需要的是将目录下所有c文件链接生成一个最后的可执行文件
# TARGETS:=$(addprefix $(BUILDDIR)/,$(BASENAMES))
TARGETS:= $(BUILDDIR)/Smarthome
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
	$(CC) $(CFLAGS) -c -emit-llvm -o $@ $<

$(BUILDDIR)/%_cfi.bc: $(BUILDDIR)/%.bc $(IGNORED_FUNCTIONS)
	$(OPT) $(OPTFLAGS) -cfi-prefix=$(basename $@) -S -o $@ $<

$(BUILDDIR)/%.o: $(BUILDDIR)/%_cfi.bc
	$(CC) $(CFLAGS) -c -o $@ $<

# 第一种尝试是每一个.bc文件均生成一个cfi.bc文件，然后统一链接
# 第二种是先生成链接好的.o文件，然后重新反汇编为统一的.bc文件，再生成cfi.bc文件，然后统一链接
$(_INJECTED)_%.c: $(BUILDDIR)/%_cfi.bc
	$(CFI_PRELINK) $(basename $<) > $@

$(_INJECTED)_%.o: $(_INJECTED)_%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGETS): $(OFILES) $(_INJECTED_OFILES)
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/DNSserver $^

clean:
	$(RM) -r $(BUILDDIR)
