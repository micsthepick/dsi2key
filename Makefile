.PHONY: clean clean_dsi client server build build_dsi

all: client server

clean:
	$(MAKE) -C client clean
	$(MAKE) -C server clean

clean_dsi:
	$(MAKE) -C client clean_dsi

client:
	$(MAKE) -C client

build_dsi:
	$(MAKE) -C client build_dsi

server:
	$(MAKE) -C server

build: client server

copy-to-SD-root: client
	mkdir -p copy-to-SD-root/dsi2key
	cp client/dsi2key.dsi client/3DS2Key.3dsx copy-to-SD-root/
	cp client/ds2key.ini.example copy-to-SD-root/ds2key/ds2key.ini
	touch copy-to-SD-root/ds2key/ds2key.log

copy-to-SD-root.zip: client copy-to-SD-root
	zip -r copy-to-SD-root.zip copy-to-SD-root

ds2key-linux-server: server
	mkdir -p ds2key-linux-server
	cp server/ds2key ds2key-linux-server
	cp server/ds2key.p0.ini.example ds2key-linux-server
	cp server/ds2key.p0.ini.example ds2key-linux-server/ds2key.p0.ini

ds2key-linux-server.tar.gz: server ds2key-linux-server
	tar cvzf ds2key-linux-server

build-packages: client server copy-to-SD-root.zip ds2key-linux-server.tar.gz
