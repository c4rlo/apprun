# runapp

_runapp_ is an application runner for Linux desktop environments that integrate with systemd.

Its job is simple: it launches the given application in an appropriate systemd user unit, using
`systemd-run`, according to systemd's
[idea](https://systemd.io/DESKTOP_ENVIRONMENTS/#xdg-standardization-for-applications) of how this
should be done.

It is useful if you are using a light-weight Linux desktop such as [Sway](https://swaywm.org/). In
such cases, `runapp` works great as a complement to your favourite application launcher, such as
[Fuzzel](https://codeberg.org/dnkl/fuzzel).

`runapp` owes its inspiration to [uwsm](https://github.com/Vladimir-csp/uwsm), a great way to go
all-in on the systemd way of running applications in a Desktop context, and another great complement
to `runapp`. In fact, the `uwsm app` subcommand has exactly the same purpose as `runapp`, and has
several more features; see below [Missing features](#missing-features) section for a comparison.

So why `runapp`? Its advantage over `uwsm app` is that it is a native executable (written in C++),
hence it has significantly lower startup latency and resource usage (`uwsm` is written in Python).
And compared to the `uwsm-app` tool that is shipped with `uwsm`, `runapp` does not require a
permanently-running background daemon.

## Usage

If you use [Sway](https://swaywm.org/) with [Fuzzel](https://codeberg.org/dnkl/fuzzel), you might
include something like the following in your `.config/sway/config` file:

```
bindsym Mod4+d runapp fuzzel --launch-prefix runapp
```

This will launch Fuzzel via `runapp` when the `Super+D` key combination is pressed, and tells Fuzzel
to also run any application it launches via `runapp`.

Variants of the above will apply for other combinations of desktop compositors and application
launchers.

## Installation

You need a recent GCC and glibc. No other libraries are required.

```
make install
```

Will compile a release version and install it (via `sudo`) as `/usr/local/bin/runapp`.

## Missing features

Most of the features on this list are implemented in `uwsm app`.

- Unit name: enforce restricted character set and max length
- Unit description: derive from `.desktop` file
  - For Fuzzel, would be made much easier (and more performant) with https://codeberg.org/dnkl/fuzzel/issues/292
- Support being given a Desktop File ID (only)
  - With optional Action ID
- Support running `.desktop` files with args, using field codes
  - See https://specifications.freedesktop.org/desktop-entry-spec/latest/exec-variables.html
  - Note that `%f` and `%u` entail running multiple app instances
  - Alternatively, this could be done by the launcher, e.g. Fuzzel: https://codeberg.org/dnkl/fuzzel/issues/346
- Support running app as systemd scope instead of systemd service
- Support running app within different systemd slices
- Support running app under Terminal
- Support custom unit name / description
- Help output
- Verbose mode
- Alternative ways of accepting Desktop File ID (beyond Fuzzel)
- Linux distro packaging: at least AUR for Arch Linux
