### This Makefile was written for GNU Make. ###
ifeq ($(OPT),true)
	COPTFLAGS  := -flto -Ofast -march=native -DNDEBUG
	LDOPTFLAGS := -flto -Ofast -s
else
ifeq ($(DEBUG),true)
	COPTFLAGS  := -O0 -g3 -ftrapv -fstack-protector-all -D_FORTIFY_SOURCE=2
	LDLIBS     := -lssp
else
	COPTFLAGS  := -O3 -DNDEBUG
	LDOPTFLAGS := -O3 -s
endif
endif
C_WARNING_FLAGS := -Wall -Wextra -Wformat=2 -Wstrict-aliasing=2 \
                   -Wcast-align -Wcast-qual -Wconversion \
                   -Wfloat-equal -Wpointer-arith -Wswitch-enum \
                   -Wwrite-strings -pedantic
ifeq ($(USE_CURSES),true)
    MACROS += -D__TU_USE_CURSES__
endif

CC      := gcc
AR      := ar
CFLAGS  := -pipe $(C_WARNING_FLAGS) $(COPTFLAGS) $(MACROS)
LDFLAGS := -pipe $(LDOPTFLAGS)
ARFLAGS := rcs

DST_DIR := lib
SRC_DIR := src
INC_DIR := include

TARGET  := $(DST_DIR)/libtermutil.a
OBJ     := $(SRC_DIR)/termutil.o
SRC     := $(OBJ:%.o=%.c)
HEADER  := $(INC_DIR)/termutil.h


.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJ)
	@if [ ! -d $(dir $@) ]; then \
		mkdir $(dir $@); \
	fi
	$(AR) $(ARFLAGS) $@ $^

$(OBJ): $(SRC) $(HEADER)



.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJ)
.PHONY: cleanobj
cleanobj:
	$(RM) $(OBJ)
