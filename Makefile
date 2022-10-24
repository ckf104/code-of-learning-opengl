SHELL := /bin/bash

HELLO_SRC := hello.cpp texture.cpp shader.cpp glad/src/glad.c
CC := g++
CFLAGS := -I./glad/include/ -I/home/ckf/glfw-3.3.8/install/include/ -L/home/ckf/glfw-3.3.8/install/lib/ -lGL -lglfw -ldl
TARGET := hello

hello: ${HELLO_SRC} 
	${CC} -g -o ${TARGET} ${HELLO_SRC} ${CFLAGS}


