#!/bin/sh
# requires apt-get install libxtst-dev
g++ -std=c++11 -DEMULATOR=0 -DD2KSERVER -DNULL_VALUE=0 -I.. main.cpp ../common/*.cpp ../common/iniparser/src/*.c core/*.cpp -lX11 -lXtst -ods2key \
&& echo "copying build to ../out" \
&& mkdir -p ../out/COPY_CONTENTS_TO_SERVER_PC \
&& cp ds2key.p0.ini.example ../out/COPY_CONTENTS_TO_SERVER_PC/ds2key.p0.ini \
&& cp ds2key.p0.ini.example ../out/COPY_CONTENTS_TO_SERVER_PC \
&& cp ds2key ../out/COPY_CONTENTS_TO_SERVER_PC/ds2key
