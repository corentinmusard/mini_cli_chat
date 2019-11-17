#Set this to @ to keep the makefile quiet
SILENCE = @

COMPONENT_NAME = run_all

ifeq "$(CPPUTEST_HOME)" ""
    CPPUTEST_HOME = ~/git-repo/cpputest
endif

SRC_DIRS += ../src

TEST_SRC_DIRS += .

CPPUTEST_CFLAGS += -D_GNU_SOURCE
CPPUTEST_CXXFLAGS += -D_GNU_SOURCE

# Look at $(CPPUTEST_HOME)/build/MakefileWorker.mk for more controls
include $(CPPUTEST_HOME)/build/MakefileWorker.mk
