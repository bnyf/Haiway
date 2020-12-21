cdir=`cd $(dirname $0); pwd`;
export HAIWAY=$cdir/../../..

${HAIWAY}/kernel/vendors/espressif/esp-idf/tools/idf.py monitor