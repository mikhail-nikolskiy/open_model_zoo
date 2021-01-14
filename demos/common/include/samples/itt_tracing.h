// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <ittnotify.h>

extern __itt_domain *itt_domain;

class ITTTask {
public:
    ITTTask(const char *name) {
        taskBegin(name);
    }

    ITTTask(const std::string &name) {
        taskBegin(name.c_str());
    }

    ~ITTTask() {
        taskEnd();
    }

    void taskBegin(const char *name) {
        if (itt_domain == nullptr) {
            itt_domain = __itt_domain_create("video-analytics");
        }
        __itt_task_begin(itt_domain, __itt_null, __itt_null, __itt_string_handle_create(name));
    }

    void taskEnd() {
        if (itt_domain)
            __itt_task_end(itt_domain);
    }
};

#define ITT_TASK(NAME) ITTTask task(NAME)
