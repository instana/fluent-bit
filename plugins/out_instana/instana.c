/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Fluent Bit
 *  ==========
 *  Copyright (C) 2019-2021 The Fluent Bit Authors
 *  Copyright (C) 2015-2018 Treasure Data Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <fluent-bit/flb_output_plugin.h>
#include <fluent-bit/flb_utils.h>
#include <fluent-bit/flb_slist.h>
#include <fluent-bit/flb_time.h>
#include <fluent-bit/flb_pack.h>
#include <fluent-bit/flb_config_map.h>
#include <fluent-bit/flb_metrics.h>

#include <msgpack.h>
#include "instana.h"

#define FLB_OUT_INSTANA_OUT_FORMAT FLB_PACK_JSON_FORMAT_LINES

static int cb_instana_init(struct flb_output_instance *ins,
                          struct flb_config *config,
                          void *data)
{
    int ret;
    struct flb_instana *ctx = NULL;
    (void) ins;
    (void) config;
    (void) data;

    ctx = flb_calloc(1, sizeof(struct flb_instana));
    if (!ctx) {
        flb_errno();
        return -1;
    }
    ctx->ins = ins;

    ret = flb_output_config_map_set(ins, (void *) ctx);
    if (ret == -1) {
        flb_free(ctx);
        return -1;
    }

    /* Export context */
    flb_output_set_context(ins, ctx);

    return 0;
}

static void cb_instana_flush(const void *data,
                             size_t bytes,
                             const char *tag,
                             int tag_len,
                             struct flb_input_instance *ins,
                             void *out_context,
                             struct flb_config *config)
{
    // struct flb_instana *ctx = out_context;
    flb_sds_t json;
    flb_sds_t date_key = flb_sds_create("date");
    (void) config;


    json = flb_pack_msgpack_to_json_format(data,
                                           bytes,
                                           FLB_OUT_INSTANA_OUT_FORMAT,
                                           FLB_PACK_JSON_DATE_ISO8601,
                                           date_key);
    write(STDOUT_FILENO, json, flb_sds_len(json));
    flb_sds_destroy(date_key);
    flb_sds_destroy(json);

    fflush(stdout);

    FLB_OUTPUT_RETURN(FLB_OK);
}

static int cb_instana_exit(void *data, struct flb_config *config)
{
    struct flb_instana *ctx = data;

    if (!ctx) {
        return 0;
    }

    flb_free(ctx);
    return 0;
}

/* Configuration properties map */
static struct flb_config_map config_map[] = {
    {
        FLB_CONFIG_MAP_STR, "endpoint", NULL,
        0, FLB_FALSE, 0,
        "Specifies the Instana endpoint to ship logs to."
    },
    {
        FLB_CONFIG_MAP_STR, "apikey", NULL,
        0, FLB_FALSE, 0,
        "Specifies the Instana apikey required to authenticate the log shipment."
    },
    {
        FLB_CONFIG_MAP_STR, "zone", NULL,
        0, FLB_FALSE, 0,
        "Specifies the Instana zone to associate with the log shipment."
    },
    {
            FLB_CONFIG_MAP_STR, "cluster", NULL,
            0, FLB_FALSE, 0,
            "Specifies the Instana cluster to associate with the log shipment."
    },
    /* EOF */
    {0}
};

/* Plugin registration */
struct flb_output_plugin out_instana_plugin = {
    .name         = "instana",
    .description  = "Instana Log Handling",
    .cb_init      = cb_instana_init,
    .cb_flush     = cb_instana_flush,
    .cb_exit      = cb_instana_exit,
    .flags        = 0,
    .event_type   = FLB_OUTPUT_LOGS,
    .config_map   = config_map
};
