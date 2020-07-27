BUILDDIR?=build

SRCDIR:=src
# SRC包含src文件中的所有文件名（包含路径）
SRC:=$(wildcard $(SRCDIR)/*.c)
OFILES:=$(patsubst %.c,%.o,$(SRC))

CFI_INCLUDE?=/usr/include

# 关闭栈保护和NX Bit
CC:=clang
CFLAGS+=-I$(CFI_INCLUDE)
CFLAGS+=-Wall -Wextra -Wtype-limits -Wsign-compare -fno-stack-protector -lpthread -lssl -lcrypto -z execstack -DCFI_DEBUG
LD:=$(CC)

# BASENAME删除了文件名中的路径，只保留文件名。basename取出文件扩展名之前的部分。
BASENAMES:=$(patsubst $(SRCDIR)/%,%,$(basename $(SRC)))
# TARGET用于生成最终编译的目标文件，这里的模版是为所有的源文件均生成可执行文件，我们需要的是将目录下所有c文件链接生成一个最后的可执行文件
# TARGETS:=$(addprefix $(BUILDDIR)/,$(BASENAMES))
TARGETS:= $(BUILDDIR)/DNSserver
MKDIR?=mkdir

.PHONY: all clean

all: clean $(TARGETS)

$(BUILDDIR):
	$(MKDIR) -p $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/DNSserver: $(OFILES)
	clang  $(LDFLAGS) -o $(BUILDDIR)/DNSserver $^

clean:
	$(RM) -r $(BUILDDIR)
