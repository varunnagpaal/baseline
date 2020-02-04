# Copyright (c) 2019, University of Washington All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
# 
# Redistributions of source code must retain the above copyright notice, this list
# of conditions and the following disclaimer.
# 
# Redistributions in binary form must reproduce the above copyright notice, this
# list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
# 
# Neither the name of the copyright holder nor the names of its contributors may
# be used to endorse or promote products derived from this software without
# specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# TODO: Makefile comment
ORANGE=\033[0;33m
RED=\033[0;31m
NC=\033[0m

################################################################################
# Paths / Environment
################################################################################
_REPO_ROOT ?= $(shell git rev-parse --show-toplevel)

-include $(_REPO_ROOT)/environment.mk


################################################################################
# Define the default kernel source file. If it is not defined, set it
# to kernel.cpp
################################################################################
KERNEL_DEFAULT ?= kernel.cpp

################################################################################
# Kernel Objects
################################################################################
# KERNEL_OBJECTS defines the object files that that are linked as part of
# the kernel. It is derived from KERNEL_*LIBRARIES (see below) but other
# objects can be added and linked as necessary.
KERNEL_OBJECTS += $(KERNEL_SLIBRARIES:.s=.rvo)
KERNEL_OBJECTS += $(KERNEL_CLIBRARIES:.c=.rvo)
KERNEL_OBJECTS += $(KERNEL_CXXLIBRARIES:.cpp=.rvo)

################################################################################
# Kernel Compilation Rules
################################################################################
_KERNEL_COMPILER ?= GCC
-include $(FRAGMENTS_PATH)/kernel/compile.mk

################################################################################
# Linker Rules
################################################################################
-include $(FRAGMENTS_PATH)/kernel/link.mk

cudalite.clean: kernel.link.clean kernel.compile.clean
