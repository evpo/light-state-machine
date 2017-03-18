.PHONY: uri_parser tests cli gtest
all:: uri_parser cli
gtest::
	$(MAKE) -C tests -f Makefile.gtest RELEASE=$(RELEASE)

tests:: gtest uri_parser
	$(MAKE) -C tests RELEASE=$(RELEASE)

cli:: uri_parser
	$(MAKE) -C cli RELEASE=$(RELEASE)

uri_parser::
	$(MAKE) -C uri_parser RELEASE=$(RELEASE)

clean::
	$(MAKE) -C tests -f Makefile.gtest clean RELEASE=$(RELEASE)
	$(MAKE) -C tests clean RELEASE=$(RELEASE)
	$(MAKE) -C cli clean RELEASE=$(RELEASE)
	$(MAKE) -C uri_parser clean RELEASE=$(RELEASE)
	$(MAKE) -C light_state_machine clean RELEASE=$(RELEASE)
	$(MAKE) -C state_machine_client clean RELEASE=$(RELEASE)
