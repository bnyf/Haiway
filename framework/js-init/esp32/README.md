esp32

partition table: /Users/bnyf/Documents/code/Haiway/kernel/vendors/espressif/boards/esp32/aws_demos/partition-table.csv

toolchain:
export PATH="/Users/bnyf/Documents/code/xtensa-esp32-elf/bin:$PATH"

cmake:
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=../../../kernel/tools/cmake/toolchains/xtensa-esp32.cmake -GNinja

build & flash:
cmake --build build --target flash

monitor:
../../../kernel/vendors/espressif/esp-idf/tools/idf.py monitor
