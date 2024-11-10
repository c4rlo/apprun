#include <stdlib.h>
#include <string.h>
#include <sys/random.h>
#include <unistd.h>

#include <cerrno>
#include <cinttypes>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>


namespace {
const char DFID_ENV_NAME[] = "FUZZEL_DESKTOP_FILE_ID";
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " COMMAND...\n";
        return 2;
    }

    std::string appName;
    if (const char* dfid = std::getenv(DFID_ENV_NAME)) {
        const char ext[] = ".desktop";
        const std::size_t extLen = sizeof(ext) - 1;
        if (std::strlen(dfid) > extLen) {
            const std::size_t dfidLen = std::strlen(dfid);
            if (std::strcmp(dfid + dfidLen - extLen, ext) == 0) {
                appName.assign(dfid, dfidLen - extLen);
            }
        }
        ::unsetenv(DFID_ENV_NAME);
    }
    if (appName.empty()) {
        appName = ::basename(argv[1]);
    }

    std::uint64_t randU64;
    if (::getentropy(&randU64, sizeof randU64) != 0) {
        std::cerr << "Failed to get random bytes: " << ::strerror(errno) << " ("
                  << ::strerrorname_np(errno) << ")\n";
        return 1;
    }

    std::string dashDE;
    if (const char* xcd = std::getenv("XDG_CURRENT_DESKTOP")) {
        dashDE.push_back('-');
        dashDE.append(xcd, ::strchrnul(xcd, ':') - xcd);
    }

    char unitArg[256]{};
    std::snprintf(unitArg, sizeof unitArg, "--unit=app%s-%s@%016" PRIx64 ".service",
                  dashDE.c_str(), appName.c_str(), randU64);

    char descriptionArg[256]{};
    std::snprintf(descriptionArg, sizeof descriptionArg, "--description=%s",
                  appName.c_str());

    std::vector<const char*> args{"systemd-run",
                                  "--user",
                                  &unitArg[0],
                                  &descriptionArg[0],
                                  "--service-type=exec",
                                  "--property=ExitType=cgroup",
                                  "--same-dir",
                                  "--slice=app-graphical.slice",
                                  "--collect",
                                  "--quiet",
                                  "--"};
    args.reserve(args.size() + argc);
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    args.push_back(nullptr);

    ::execvp(args.front(), const_cast<char *const *>(args.data()));

    std::cerr << "Failed to execute systemd-run: " << ::strerror(errno) << " ("
              << ::strerrorname_np(errno) << ")\n";
    return 1;
}
