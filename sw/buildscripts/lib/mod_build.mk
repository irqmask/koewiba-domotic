include targets.mk

DEBUGDIRS = $(SUBDIRS:%=debug-%)
RELEASEDIRS = $(SUBDIRS:%=release-%)

all: debug release

debug: $(DEBUGDIRS)

$(DEBUGDIRS):
	@make -f Makefile build_debug -s -C $(@:debug-%=%)

release: $(RELEASEDIRS)

$(RELEASEDIRS):
	@make -f Makefile build_release -s -C $(@:release-%=%)

helpbuild:
	@echo ""
	@echo "    List of build-targets:"
	@echo ""
	@echo "        all                 - builds debug and release"
	@echo ""
	@echo "        debug               - builds debug"
	@echo ""
	@echo "        release             - builds release"
	@echo ""


.PHONY: all debug release helpbuild