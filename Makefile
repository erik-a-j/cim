SRCDIR := src
OBJDIR := build

CC      := gcc
CFLAGS  := -Wall -Wextra -Winline -pedantic -fanalyzer -Wanalyzer-null-dereference -g -O0 -std=gnu11 -MMD -MP
LDFLAGS :=
LDLIBS  :=

# source/object lists
SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
DEPS := $(OBJ:.o=.d)

.PHONY: all clean temps run
all: cim

# final binary
cim: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)


# compile rule — ensure OBJDIR exists first (order-only prereq)
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# create build dir as needed
$(OBJDIR):
	@mkdir -p $@

run: cim
	./cim

clean:
	rm -f cim $(OBJDIR)/*.o $(OBJDIR)/*.i $(OBJDIR)/*.s $(OBJDIR)/*.d

# special build that saves temps (.i/.s) next to each .o
temps:
	@$(MAKE) clean
	@$(MAKE) CFLAGS="$(CFLAGS) -save-temps=obj"

# include auto-generated header dependency files
-include $(DEPS)