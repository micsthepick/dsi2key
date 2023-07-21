make -f Makefile_nds
echo "copying build to ../out"
mkdir -p ../out/COPY_CONTENTS_TO_SD_ROOT
cp ds2key.ini.example ../out/COPY_CONTENTS_TO_SD_ROOT/ds2key.ini
cp ds2key.nds ../out/COPY_CONTENTS_TO_SD_ROOT