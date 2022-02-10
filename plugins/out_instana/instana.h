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

#ifndef FLB_OUT_INSTANA
#define FLB_OUT_INSTANA

#include <fluent-bit/flb_output_plugin.h>
#include <fluent-bit/flb_sds.h>

struct flb_instana {
    /**
     * Instana Backend Parameters
     * Agent Reference: https://www.ibm.com/docs/en/obi/current?topic=agent-host-configuration#custom-zones
     * */

    /* Instana API key */
    flb_sds_t apikey;
    /* Instana acceptor endpoint in the format of host:port */
    flb_sds_t endpoint;

    // TODO (2022-02-16): Optionally add split reporting.

    /* Customer Configuration */
    /* Customer Kubernetes cluster name */
    flb_sds_t cluster;
    /* Customer custom zone name */
    flb_sds_t zone;

    /* Proxy Configuration. */
    flb_sds_t proxyHost;
    flb_sds_t proxyPort;
    /* Proxy protocol is one of: http, socks4, or socks5. */
    flb_sds_t proxyProtocol;
    flb_sds_t proxyUser;
    flb_sds_t proxyPassword;
    /* TODO: Check if this is possible with fluent-bit's library. */
    flb_sds_t proxyUseDNS;

    struct flb_output_instance *ins;
};

#endif
