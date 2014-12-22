!if "$(CRTDLL)" == "true"
CRTLIB = /MD$(DBG_SUFFIX)
!else
CRTLIB = /MT$(DBG_SUFFIX)
!endif

!if "$(DEBUG)" == "true"
COPTFLAGS   = /Od /GS /Zi $(CRTLIB)
AROPTFLAGS  =
MSVC_MACROS = /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_WARNINGS \
              /D_USE_MATH_DEFINES
DBG_SUFFIX  = d
!else
COPTFLAGS   = /Ox /GL $(CRTLIB)
AROPTFLAGS  = /LTCG
MSVC_MACROS = /DNDEBUG /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_WARNINGS \
              /D_USE_MATH_DEFINES
DBG_SUFFIX  =
!endif

CC = cl
AR = lib
RM = del /F

MACROS  = $(MSVC_MACROS)
CFLAGS  = /nologo $(COPTFLAGS) /W4 /c $(MACROS)
ARFLAGS = /nologo $(AROPTFLAGS)

DST_DIR = lib
SRC_DIR = src
INC_DIR = include

TARGET  = $(DST_DIR)\termutil$(DBG_SUFFIX).lib
OBJ     = $(SRC_DIR)\termutil$(DBG_SUFFIX).obj
SRC     = $(SRC_DIR)\termutil.c
HEADER  = $(INC_DIR)\termutil.h


all: $(TARGET)

$(TARGET): $(OBJ)
	@if not exist $(@D)\NUL \
		mkdir $(@D)
	$(AR) $(ARFLAGS) $** /OUT:$@

$(OBJ): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) $** /c /Fo$@


clean:
	$(RM) $(TARGET) $(OBJ)
cleanobj:
	$(RM) $(OBJ)
