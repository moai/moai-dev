/*
 * Copyright 2008, Dave Benson.
 * Copyright 2008 - 2009 Plausible Labs Cooperative, Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with
 * the License. You may obtain a copy of the License
 * at http://www.apache.org/licenses/LICENSE-2.0 Unless
 * required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#import "PLCrashAsync.h"

typedef enum {
        PLPROTOBUF_C_TYPE_INT32,
        PLPROTOBUF_C_TYPE_SINT32,
        PLPROTOBUF_C_TYPE_SFIXED32,
        PLPROTOBUF_C_TYPE_INT64,
        PLPROTOBUF_C_TYPE_SINT64,
        PLPROTOBUF_C_TYPE_SFIXED64,
        PLPROTOBUF_C_TYPE_UINT32,
        PLPROTOBUF_C_TYPE_FIXED32,
        PLPROTOBUF_C_TYPE_UINT64,
        PLPROTOBUF_C_TYPE_FIXED64,
        PLPROTOBUF_C_TYPE_FLOAT,
        PLPROTOBUF_C_TYPE_DOUBLE,
        PLPROTOBUF_C_TYPE_BOOL,
        PLPROTOBUF_C_TYPE_ENUM,
        PLPROTOBUF_C_TYPE_STRING,
        PLPROTOBUF_C_TYPE_BYTES,
        //PLPROTOBUF_C_TYPE_GROUP,          // NOT SUPPORTED
        PLPROTOBUF_C_TYPE_MESSAGE,
} PLProtobufCType;

typedef struct PLProtobufCBinaryData {
    size_t len;
    void *data;
} PLProtobufCBinaryData;

size_t plcrash_writer_pack (plcrash_async_file_t *file, uint32_t field_id, PLProtobufCType field_type, const void *value);