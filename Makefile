SRCDIR := src
OBJDIR := build

# ---- Toolchain selection -----------------------------------------------------
TOOLCHAIN ?= native
ARCH ?= x86_64

CFLAGS  := -Wall -Wextra -Winline -pedantic -fanalyzer -Wanalyzer-null-dereference -g -O0 -std=gnu11 -MMD -MP
LDFLAGS :=
LDLIBS  :=

ifeq ($(TOOLCHAIN),mingw)
CROSS_PREFIX := $(ARCH)-w64-mingw32-
CC      := $(CROSS_PREFIX)gcc
STRIP   := $(CROSS_PREFIX)strip
EXEEXT  := .exe
CFLAGS  += -m64 -D_WIN32 -D_WIN32_WINNT=0x0A00 -D__USE_MINGW_ANSI_STDIO=1
else
CC      := gcc
EXEEXT  :=
endif

# ---- sources/objects (include top-level and one-level subdirs) --------------
SRC  := $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*/*.c)
OBJ  := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
DEPS := $(OBJ:.o=.d)

# Final binary
BIN := main$(EXEEXT)

.PHONY: all clean temps run windows64 windows32 strip
all: $(BIN)

# ---- link --------------------------------------------------------------------
$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# ---- compile (ensure OBJDIR exists first) -----------------------------------
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(INC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	@mkdir -p $@

# ---- convenience targets -----------------------------------------------------
run: $(BIN)
	@if [ "$(TOOLCHAIN)" = "mingw" ]; then \
		echo "Running with Wine…"; \
		wine $(BIN); \
	else \
		./$(BIN); \
	fi

strip: $(BIN)
	@if [ -n "$(STRIP)" ]; then $(STRIP) -s $(BIN); else strip -s $(BIN); fi

clean:
	rm -f $(BIN) $(OBJDIR)/*.o $(OBJDIR)/*/*.o $(OBJDIR)/*.i $(OBJDIR)/*/*.i $(OBJDIR)/*.s $(OBJDIR)/*/*.s $(OBJDIR)/*.d $(OBJDIR)/*/*.d

temps:
	@$(MAKE) clean
	@$(MAKE) CFLAGS="$(CFLAGS) -save-temps=obj"

# include auto-generated header dependency files
-include $(DEPS)

# ---- helper wrappers for cross builds ---------------------------------------
windows64:
	@$(MAKE) clean
	@$(MAKE) TOOLCHAIN=mingw ARCH=x86_64

windows32:
	@$(MAKE) clean
	@$(MAKE) TOOLCHAIN=mingw ARCH=i686
