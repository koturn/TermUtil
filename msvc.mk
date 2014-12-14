CC = cl
AR = lib
RM = del /F

MACROS  = /D_CRT_SECURE_NO_WARNINGS /DNDEBUG
CFLAGS  = /nologo /O2 /W4 /c $(MACROS)
LDFLAGS = /nologo /O2
ARFLAGS = /nologo

DST_DIR = lib
SRC_DIR = src
INC_DIR = include

TARGET  = $(DST_DIR)\termutil.lib
OBJ     = $(SRC_DIR)\termutil.obj
SRC     = $(OBJ:.obj=.c)
HEADER  = $(INC_DIR)\termutil.h


.SUFFIXES: .c .obj
.c.obj:
	$(CC) $(CFLAGS) $** /c /Fo$@


all: $(TARGET)

$(TARGET): $(OBJ)
	@if not exist $(@D)\NUL \
		mkdir $(@D)
	$(AR) $(ARFLAGS) $** /OUT:$@

$(OBJ): $(SRC) $(HEADER)


clean:
	$(RM) $(TARGET) $(OBJ)
cleanobj:
	$(RM) $(OBJ)
