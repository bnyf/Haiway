curdir=`pwd`
cd ../../..
cdir=`cd $(dirname $0); pwd`;
export HAIWAY=$cdir
cd $curdir

echo "${HAIWAY}"

OSNAME=${1:-MAC}
echo "os: ${OSNAME}"
if ! type xtensa-esp32-elf-gcc 2> /dev/null > /dev/null; then
    echo installing xtensa-esp32-elf-gcc
    if [ ! -d "xtensa-esp32-elf" ]; then
    if [ "$OSNAME" = "LINUX" ]; then
            curl -L https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz | tar xfz - --no-same-owner
    fi
    if [ "$OSNAME" = "MAC" ]; then
            curl -L https://dl.espressif.com/dl/xtensa-esp32-elf-osx-1.22.0-80-g6c4433a-5.2.0.tar.gz | tar xfz - --no-same-owner
    fi
    else
        echo "Folder found"
    fi
    echo "export `pwd`/xtensa-esp32-elf/bin/"
    export PATH=$PATH:`pwd`/xtensa-esp32-elf/bin/
fi

cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=${HAIWAY}/kernel/tools/cmake/toolchains/xtensa-esp32.cmake

cmake --build build --target flash