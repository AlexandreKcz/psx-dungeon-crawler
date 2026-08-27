TARGET = Dungeon-Crawler
TYPE = ps-exe

SRCS = \
third_party/nugget/common/crt0/crt0.s \
main.c \
src/engine/engine.c \
src/engine/system/system.c \
src/engine/render/render.c \
src/engine/types/types.c \
src/engine/types/array_list.c \
src/engine/prims/prims.c \
src/engine/controller/controller.c \
src/engine/cd/cd.c \
src/engine/sprite/sprite.c \
src/engine/light/light.c \
src/engine/camera/camera.c \
src/game/dungeon-crawler/dungeon_crawler.c \
src/game/game.c

CPPFLAGS += -Ithird_party/psyq-iwyu/include
LDFLAGS += -Lthird_party/psyq/lib
LDFLAGS += -Wl,--start-group
LDFLAGS += -lapi
LDFLAGS += -lc
LDFLAGS += -lc2
LDFLAGS += -lcard
LDFLAGS += -lcd
LDFLAGS += -lcomb
LDFLAGS += -lds
LDFLAGS += -letc
LDFLAGS += -lgpu
LDFLAGS += -lgs
LDFLAGS += -lgte
LDFLAGS += -lgun
LDFLAGS += -lhmd
LDFLAGS += -lmath
LDFLAGS += -lmcrd
LDFLAGS += -lmcx
LDFLAGS += -lpad
LDFLAGS += -lpress
LDFLAGS += -lsio
LDFLAGS += -lsnd
LDFLAGS += -lspu
LDFLAGS += -ltap
LDFLAGS += -Wl,--end-group

include third_party/nugget/common.mk
