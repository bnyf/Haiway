# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License").
# You may not use this file except in compliance with the License.
# A copy of the License is located at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# or in the "license" file accompanying this file. This file is distributed
# on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
# express or implied. See the License for the specific language governing
# permissions and limitations under the License.

default: $(OUT_DIR)/all.txt

$(OUT_DIR)/1.txt:
	$(LITANI) add-job       \
	  --outputs $@          \
	  --ci-stage report     \
	  --pipeline 1          \
	  --command "touch $@"  \
	  #

$(OUT_DIR)/2.txt:
	$(LITANI) add-job       \
	  --outputs $@          \
	  --ci-stage test       \
	  --pipeline 2          \
	  --command "touch $@"  \
	  #

$(OUT_DIR)/3.txt:
	$(LITANI) add-job       \
	  --outputs $@          \
	  --ci-stage build      \
	  --pipeline 3          \
	  --command "touch $@"  \
	  #

$(OUT_DIR)/all.txt: $(OUT_DIR)/1.txt $(OUT_DIR)/2.txt $(OUT_DIR)/3.txt
	$(LITANI) add-job     \
	  --inputs $^         \
	  --outputs $@        \
	  --stdout-file $@    \
	  --ci-stage report   \
	  --pipeline all      \
	  --command "cat $^"  \
	  #
