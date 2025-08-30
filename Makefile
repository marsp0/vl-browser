# tab here: [	]

# ------------------- files -------------------
SRC_FILES       := $(shell find src -name "*.c" ! -name "main.c")
OBJ_FILES       := $(addprefix out/, $(SRC_FILES:.c=.o))

TEST_SRC_FILES   := $(shell find ./test -name "*.c")
TEST_OBJ_FILES   := $(addprefix out/, $(TEST_SRC_FILES:.c=.o))


# ------------------- compiler -------------------
GCC_INCLUDE := -I./src/
GCC_FLAGS   := -std=gnu11 -Wall -Wextra -Werror -Wshadow -Wpedantic 
GCC_FLAGS   += -Wnull-dereference -Wunused -Wconversion -Wno-pointer-sign
LD_FLAGS    := -lpthread -lm -lrt -ldl

ifeq ($(debug), 1)
	GCC_FLAGS  += -g -O0 -fprofile-arcs -ftest-coverage
	LD_FLAGS   += -lgcov --coverage
else
	GCC_FLAGS  += -O2
endif


# ------------------- targets -------------------
.PHONY: clean vl test_vl


# ------------------- targets: vl -------------------
vl: main.o $(OBJ_FILES)
	$(CC) $(GCCFLAGS) ./out/main.o $(OBJ_FILES) -o ./out/$@ $(LD_FLAGS)

main.o: ./src/main.c
	$(CC) $(GCC_FLAGS) $(GCC_INCLUDE) -c $< -o ./out/$@


# ------------------- targets: test -------------------
test_vl: GCC_INCLUDE += -I./test
test_vl: $(TEST_OBJ_FILES) $(OBJ_FILES)
	$(CC) $(GCCFLAGS) $(TEST_OBJ_FILES) $(OBJ_FILES) -o ./out/$@ $(LD_FLAGS)


# ------------------- targets: obj files -------------------
out/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(GCC_FLAGS) $(GCC_INCLUDE) -c $< -o $@


# ------------------- targets: clean -------------------
clean:
	@rm -rf out/*
