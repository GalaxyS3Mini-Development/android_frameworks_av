// Copyright 2015, The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/unique_fd.h>

#include <libminijail.h>
#include <scoped_minijail.h>

#include "minijail.h"

namespace android {

int WritePolicyToPipe(const std::string& base_policy_content,
                      const std::string& additional_policy_content)
{
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        PLOG(ERROR) << "pipe() failed";
        return -1;
    }

    base::unique_fd write_end(pipefd[1]);
    std::string content = base_policy_content;

    if (additional_policy_content.length() > 0) {
        content += "\n";
        content += additional_policy_content;
    }

    if (!base::WriteStringToFd(content, write_end.get())) {
        LOG(ERROR) << "Could not write policy to fd";
        return -1;
    }

    return pipefd[0];
}

void SetUpMinijail(const std::string& base_policy_path __unused, const std::string& additional_policy_path __unused)
{
    LOG(WARNING) << "No seccomp policy defined for this architecture.";
    return;
}
}
