esp32

cmake:
cmake -S . -B build -GNinja

build & flash:
cmake --build build --target flash

monitor:
${HAIWAY}/kernel/vendors/espressif/esp-idf/tools/idf.py monitor

partition table:
${HAIWAY}/kernel/vendors/espressif/boards/esp32/aws_demos/partition-table.csv