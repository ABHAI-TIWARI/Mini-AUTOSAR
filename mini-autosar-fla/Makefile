# Makefile – Mini-AUTOSAR FLA
# Fallback for environments without CMake.
# Usage:
#   make          → build demo binary
#   make tests    → build + run all unit tests
#   make demo     → build + run demo
#   make clean    → remove build artifacts

CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -Wno-unused-parameter -I.
OUTDIR  = build

SRCS_COMMON = \
    src/mcal/Mcal_Dio.c    \
    src/mcal/Mcal_Gpt.c    \
    src/bsw/Det.c          \
    src/bsw/Dio.c          \
    src/bsw/Gpt.c          \
    src/bsw/EcuM.c         \
    src/rte/Rte.c          \
    src/app/ButtonReader.c \
    src/app/BlinkController.c \
    src/app/Heartbeat.c

DEMO_BIN   = $(OUTDIR)/mini_autosar_fla

TEST_BINS  = \
    $(OUTDIR)/test_Mcal_Dio      \
    $(OUTDIR)/test_Det           \
    $(OUTDIR)/test_ButtonReader  \
    $(OUTDIR)/test_BlinkController \
    $(OUTDIR)/test_Heartbeat

.PHONY: all demo tests clean

all: $(DEMO_BIN)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(DEMO_BIN): $(SRCS_COMMON) main.c | $(OUTDIR)
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Built $(DEMO_BIN)"

demo: $(DEMO_BIN)
	./$(DEMO_BIN)

$(OUTDIR)/test_%: tests/test_%.c tests/minitest.c $(SRCS_COMMON) | $(OUTDIR)
	$(CC) $(CFLAGS) $^ -o $@

tests: $(TEST_BINS)
	@echo ""
	@echo "===== Running all unit tests ====="
	@pass=0; fail=0; \
	for t in $(TEST_BINS); do \
	    echo ""; echo "--- $$t ---"; \
	    if ./$$t; then pass=$$((pass+1)); else fail=$$((fail+1)); fi; \
	done; \
	echo ""; echo "===== Summary: $$pass passed, $$fail failed ====="; \
	[ $$fail -eq 0 ]

clean:
	rm -rf $(OUTDIR)
