#
# \brief  Common front-end code shared among different ports tools
# \author Norman Feske
# \date   2014-05-27
#

include $(GENODE_DIR)/tool/ports/mk/common.inc

#
# Always execute the $(TARGET) rule regardless of the supplied command-line
# argument
#
TARGET := $(MAKECMDGOALS) default
$(MAKECMDGOALS) default:

# the single argument is the name of the port to prepare
ARGUMENT  := $(MAKECMDGOALS)
PORT_NAME := $(firstword $(ARGUMENT))

# compound directory where all 3rd-party source codes are installed
CONTRIB_DIR ?= $(GENODE_DIR)/contrib

# list of all repositories located at '<genode-dir>/repos/'
REPOSITORIES ?= $(shell find $(GENODE_DIR)/repos -follow -mindepth 1 -maxdepth 1 -type d)

# list of all repositories that contain ports of 3rd-party source code
_REP_PORTS_DIRS := $(wildcard $(addsuffix /ports,$(REPOSITORIES)))

# list of all available ports
_PORTS := $(foreach DIR,$(_REP_PORTS_DIRS),$(wildcard $(DIR)/*.port))

# port file to use
PORT := $(filter %/$(PORT_NAME).port,$(_PORTS))

# repository that contains the port description, used to look up patch files
REP_DIR := $(realpath $(dir $(PORT))/..)

# read hash that uniquely identifies the version to install
HASH_FILE := $(wildcard $(PORT:.port=.hash))
HASH      := $(if $(HASH_FILE),$(shell cat $(HASH_FILE)),)

# path to hash file relative to '<genode-dir>/repos', used for error messages
_REL_HASH_FILE := $(notdir $(REP_DIR))/ports/$(notdir $(PORT))

# directory where to install the port
PORT_DIR := $(CONTRIB_DIR)/$(PORT_NAME)-$(HASH)

#
# Protect the GENODE_DIR Git repository by creating an
# empty Git repository in CONTRIB_DIR. This is to catch
# Git tools called by port recipes from falling through
# the parent directories of PORT_DIR and affecting other
# Git repositories.
#
$(PORT_DIR): $(CONTRIB_DIR)/.git
$(CONTRIB_DIR)/.git:
	$(VERBOSE)mkdir -p $(CONTRIB_DIR)
	$(VERBOSE)git init --quiet $(CONTRIB_DIR)

# path to hash file generated during installation
PORT_HASH_FILE := $(PORT_DIR)/$(PORT_NAME).hash

# vi: set ft=make :
