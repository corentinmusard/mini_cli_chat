TOPTARGETS := all clean
SUBDIRS := src tests

$(TOPTARGETS): $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean: ${SUBDIRS}
	rmdir bin

.PHONY: $(TOPTARGETS) $(SUBDIRS)
