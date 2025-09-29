SRCDIR := src
OBJDIR := build

# ---- Toolchain selection -----------------------------------------------------
# Usage:
#   make                        # native (gcc)
#   make TOOLCHAIN=mingw        # cross-compile to Windows (x86_64 by default)
#   make TOOLCHAIN=mingw ARCH=i686  # 32-bit Windows
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
# Add Windows-specific libs only if you use them (examples):
# LDLIBS  += -lws2_32 -luser32 -lkernel32
else
CC      := gcc
EXEEXT  :=
endif



# ---- source/object lists -----------------------------------------------------
SRC  := $(wildcard $(SRCDIR)/*.c)
OBJ  := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
DEPS := $(OBJ:.o=.d)

# Final binary name adapts to Windows
BIN := cim$(EXEEXT)

.PHONY: all clean temps run windows64 windows32 strip
all: $(BIN)

# ---- link --------------------------------------------------------------------
$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# ---- compile (ensure OBJDIR exists first) -----------------------------------
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

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

# Strip size (esp. handy for Windows builds)
strip: $(BIN)
	@if [ -n "$(STRIP)" ]; then $(STRIP) -s $(BIN); else strip -s $(BIN); fi

clean:
	rm -f cim cim.exe $(OBJDIR)/*.o $(OBJDIR)/*.i $(OBJDIR)/*.s $(OBJDIR)/*.d

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
