# Cyra - A C++ command line parsing library
# Copyright (C) 2018 North Crescent
# 
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
# 
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <https://www.gnu.org/licenses/>.

MAKEFLAGS += --no-builtin-rules --no-builtin-variables --no-print-directory

VERSION := 0.1.0-alpha

ARCHIVE_DIRECTORY := archive
BINARY_DIRECTORY := binary
BUILD_DIRECTORY := build
INCLUDE_DIRECTORY := include
SOURCE_DIRECTORY := source

ARCHIVE_FILE := ${ARCHIVE_DIRECTORY}/libcyra.a
BINARY_FILE := ${BINARY_DIRECTORY}/test

SOURCE_FILES := $(sort $(shell find ${SOURCE_DIRECTORY} -name "*.cc"))
OBJECT_FILES := $(patsubst ${SOURCE_DIRECTORY}/%.cc,\
    ${BUILD_DIRECTORY}/%.o,${SOURCE_FILES})

DEPENDENCY_FILES := $(patsubst %.o,%.d,${OBJECT_FILES})

PARENT_DIRECTORIES := $(patsubst %/,\
    %,$(sort $(dir ${ARCHIVE_FILE} ${BINARY_FILE} ${OBJECT_FILES})))

COMPILER := clang++
COMPILER_OPTIONS := -std=c++17 -I${INCLUDE_DIRECTORY} -Wall -MMD

LINKER := ar
LINKER_OPTIONS := rcs

.SECONDEXPANSION:

release: ${ARCHIVE_FILE}

debug: COMPILER_OPTIONS += -O0 -g
debug: ${ARCHIVE_FILE}

test: COMPILER_OPTIONS += -O0 -g
test: ${BINARY_FILE}

${ARCHIVE_FILE}: ${OBJECT_FILES} | $$(@D)
	${LINKER} ${LINKER_OPTIONS} $@ $^

${BUILD_DIRECTORY}/%.o: ${SOURCE_DIRECTORY}/%.cc | $$(@D)
	${COMPILER} ${COMPILER_OPTIONS} -c -o $@ $^

${BINARY_FILE}: test.cc ${ARCHIVE_FILE} | $$(@D)
	${COMPILER} ${COMPILER_OPTIONS} -o $@ $^

${PARENT_DIRECTORIES}:
	mkdir -p $@

-include ${DEPENDENCY_FILES}

run: test
	${BINARY_FILE}

clean:
	$(if $(wildcard ${ARCHIVE_DIRECTORY}/.),rm -r ${ARCHIVE_DIRECTORY})
	$(if $(wildcard ${BINARY_DIRECTORY}/.),rm -r ${BINARY_DIRECTORY})
	$(if $(wildcard ${BUILD_DIRECTORY}/.),rm -r ${BUILD_DIRECTORY})

.PHONY: release debug test run clean
