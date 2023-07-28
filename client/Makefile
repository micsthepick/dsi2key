.PHONY: clean build build_3ds build_dsi clean_3ds clean_dsi


all: build

build_dsi:
	$(MAKE) -f Makefile_dsi

build_3ds:
	$(MAKE) -f Makefile_3ds

build: build_dsi build_3ds

clean_dsi:
	$(MAKE) -f Makefile_dsi clean

clean_3ds:
	$(MAKE) -f Makefile_3ds clean

clean: clean_dsi clean_3ds