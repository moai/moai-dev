#!/bin/bash
#
# Copyright (c) 2011 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.


readonly SCRIPT_DIR="$(dirname "$0")"
readonly SCRIPT_DIR_ABS="$(cd "${SCRIPT_DIR}" ; pwd -P)"

export NACL_SDK_ROOT=C:/NaCl/native_client_sdk_0_5_1052

readonly BASE_SCRIPT="${NACL_SDK_ROOT}/third_party/scons-2.0.1/script/scons"

export PYTHONPATH="${NACL_SDK_ROOT}/third_party/scons-2.0.1/engine"
# We have to do this because scons overrides PYTHONPATH and does not preserve
# what is provided by the OS.  The custom variable name won't be overwritten.
export PYMOX="${NACL_SDK_ROOT}/third_party/pymox"

"${BASE_SCRIPT}" --file=build.scons \
                 --site-dir="${NACL_SDK_ROOT}/build_tools/nacl_sdk_scons" \
                 $*

