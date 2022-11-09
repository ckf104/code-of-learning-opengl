SHELL := /bin/bash
SRC = hello.cpp game.cpp game_level.cpp game_object.cpp resource_manager.cpp  sprite_renderer.cpp particle.cpp texture.cpp shader.cpp
HELLO_SRC := $(addprefix src/, ${SRC}) glad/src/glad.c
CC := g++
CFLAGS = -std=c++17 -DparticleVelocity -O3 -I./include/ -I./glad/include/ -I./glm/include/ -I/home/ckf/glfw-3.3.8/install/include/ -L/home/ckf/glfw-3.3.8/install/lib/ -lGL -lglfw -ldl

FREETYPE = freetype/lib/pkgconfig/
CFLAGS += $(shell PKG_CONFIG_PATH=${FREETYPE} pkg-config --cflags --libs freetype2)

hello: ${HELLO_SRC} 
	${CC} -o $@ ${HELLO_SRC} ${CFLAGS}

.PHONY: clean
clean:
	@rm hello


