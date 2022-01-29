CFLAGS = -Wall
LIBS   = -lpthread
SRCDIR = src
OBJDIR = build
BINDIR = bin
CFILES = $(wildcard $(SRCDIR)/*.c) bfe.c
OFILES = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(CFILES))
EXEC   = $(BINDIR)/bfe

.PHONY: clean

all: $(EXEC)

$(EXEC): $(OFILES)
	@mkdir -p $(BINDIR)
	@echo "Building $(EXEC)"
	@$(CC) $(CFLAGS) $(OFILES) -o $@ $(LIBS)
	@echo "$(EXEC) built"
	@$(RM) $(OBJDIR)/*.o

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) -r $(EXEC)
