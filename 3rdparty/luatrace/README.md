# luatrace - tracing, profiling and coverage for Lua

## 1. What?

luatrace is a Lua module that collects information about what your code is doing
and how long it takes, and can analyse that information to generate profile and
coverage reports.

luatrace adds a layer on top of Lua's debug hooks to make it easier to collect
information for profiling and coverage analysis.
luatrace traces of every line executed, not just calls.

luatrace can trace through coroutine resumes and yields, and through xpcalls,
pcalls and errors.
On some platforms it uses high resolution timers to collect times of the order
of nanoseconds.

To use it, install luatrace with `sudo make install`,
run your code with `lua -luatrace <your lua file>` and then analyse it
with `luatrace.profile`.  The profiler will display a list of the top 20 functions
by time, and write a copy of all the source traced annotated with times for each
line.

Alternatively, you can `local luatrace = require("luatrace")` and surround the code
you wish to trace with `luatrace.tron()` and `luatrace.troff()`.

If you wish to use the profiler directly rather than on a trace file you can use
`lua -luatrace.profile <your lua file>` or `local luatrace = require("luatrace.profile")`.

luatrace runs under "plain" Lua and LuaJIT with the -joff option (LuaJIT doesn't
call hooks in compiled code, and luatrace loses track of where it's up to)

luatrace is brought to you by [Incremental](http://www.incremental.co.nz/) (<info@incremental.co.nz>)
and is available under the [MIT Licence](http://www.opensource.org/licenses/mit-license.php).


## 2. How?

luatrace is separated into two parts - the trace collector, and the backends that
record and process the traces.

The trace collector uses Lua's debug hooks and adds timing information and a
little bit of processing to make the traces easier to use.

Timing is provided in one of three ways:

+ Lua - with a debug hook calling `os.clock`
+ LuaJIT - with a debug hook calling `ffi.C.clock` - `os.clock` is not yet
  implemented as a fast function
+ Lua and LuaJIT - if the c_hook has been built then that's used instead of the
  Lua or LuaJIT hook.  It's always better to use the C hook.

By default the C hook uses the C library's `clock` and should call it closer to
actual code execution, so the traces should be more accurate.
On some plaforms the C hook uses a high-resolution timer:

+ On mach plaforms (ie OS X), the c_hook uses the `mach_absolute_time`
+ On Linux, it uses `clock_gettime`
+ On Windows, it uses `QueryPerformanceCounter`

However, although the timing might be collected at nanosecond resolution, there
are many reasons why profiles are not accurate to within a nanosecond!

The collector outputs traces by calling a recorder's `record` function with a
range of arguments:

+ `("S", <filename>, <line>)` - the trace has started somewhere in a function defined on line
+ `(">", <filename>, <line>)` - there's been a call to a function defined on line
+ `("T", <filename>, <line>)` - there's been a tailcall to a function defined on line (LuaJIT only)
+ `("<")` - return from a function
+ `("R", <thread_id>)` - Resume the thread thread_id
+ `("Y")` - Yield
+ `("P")` - pcall - the current line is protected for the duration of the following call
+ `("E")` - Error - unwind the stack looking for a "P"
+ `(<line>, <time in microseconds>)` - this many microseconds were just spent on this line of the current file

At the moment, there's two recorders - `luatrace.trace_file` and `luatrace.profile`.

`trace_file` is the default backend.  It just writes the trace out to a file in a simple format,
which it can also read.  When it reads a file, it reads it to another recorder
as if the recorder were watching the program execute.

`profile` provides limited profile information.

Backends will improve as I need them or as you patch/fork.


## 3. Requirements

Lua or LuaJIT.


## 4. Issues

+ It's _really_ slow
+ Tracing is overcomplicated and has to check the stack depth too frequently
+ Profiling is very complicated when there's a lot on one line (one line functions)
+ Times probably aren't accurate because of the time spent getting between user code and the hooks
+ There aren't many backends


## 5. Wishlist

+ More of the hook should be in C
+ It would be nice if the recorder was in a separate Lua state and a separate thread


## 6. Alternatives

See [the Lua Wiki](http://lua-users.org/wiki/ProfilingLuaCode) for a list of profiling alternatives.
[luacov](http://luacov.luaforge.net/) provides coverage analysis.


# `-jannotate` - Annotate code with LuaJIT trace information

## 1. What?

`annotate.lua` collects information about the traces LuaJIT is attempting and
summarises that information in a format that doesn't contain as much information
as `-jdump`, but which might be more useful for you.

`annotate.lua` is installed with luatrace.
To use it, `luajit -jannotate mylua.lua`.
When your program exits, it will write a report to stdout detailing how many
traces were attempted, how many were successful and how many were aborted.
For the aborted traces, you'll get summaries of the reasons for the aborts,
and the lines that caused the aborts.

The report then lists bytecode for all the traces next to the source code.
This listing is a little problematic, since bytecode order doesn't necessarily
match source order, and traces cross function boundaries.
The traces are listed in source order as much as practicable to make them easier
to read.

If you want to write the report to a separate file, then `luajit -jannotate=report.txt`.

You can also use annotate.lua as a module:

    local annotate = require("jit.annotate")
    annotate.on()
    ...
    annotate.off()
    annotate.report(io.open("report.txt", "w"))

## 2. Requirements

LuaJIT (this won't work with plain Lua)

## 3. Wishlist

+ It'd be nice to incorporate timing information from a profiler

## 4. Alternatives

`-jdump`, `-jbc`, `-jv` etc


# LuaJIT trace API

`-jannotate` uses some internal LuaJIT APIs.
These are not intended for public use, and are subject to change without notice,
however, here's some of what I've managed to learn.
What follows is a bit rough, inaccurate, and very incomplete.
These functions are used in several of the -j library files.
`dump.lua` is probably a good place to start if you're looking for more information

## jit.attach

You can attach callbacks to a number of compiler events with `jit.attach`.  The callback can be called:

- when a function has been compiled to bytecode ("bc");
- when trace recording starts or stops ("trace");
- as a trace is being recorded ("record");
- or when a trace exits through a side exit ("texit").

Set a callback with `jit.attach(callback, "event")` and clear the same callback with `jit.attach(callback)`

The arguments passed to the callback depend on the event being reported:

- "bc": `callback(func)`. `func` is the function that's just been recorded.
- "trace": `callback(what, tr, func, pc, otr, oex)`
    - `what` is a description of the trace event: "flush", "start", "stop", "abort".  Available for all events.
    - `tr` is the trace number.  Not available for flush.
    - `func` is the function being traced.  Available for start and abort.
    - `pc` is the program counter - the bytecode number of the function being recorded (if this a Lua function).  Available for start and abort.
    - `otr` start: the parent trace number if this is a side trace, abort: abort code (integer)?
    - `oex` start: the exit number for the parent trace, abort: abort reason (string) 
- "record": `callback(tr, func, pc, depth)`. The first arguments are the same as for trace start. `depth` is the depth of the inlining of the current bytecode.
- "texit": `callback(tr, ex, ngpr, nfpr)`.
    - `tr` is the trace number as before
    - `ex` is the exit number
    - `ngpr` and `nfpr` are the number of general-purpose and floating point registers that are active at the exit.


## jit.util.funcinfo(func, pc)

When passed `func` and `pc` from a `jit.attach` callback,
`jit.util.funcinfo` returns a table of information about the function,
much like `debug.getinfo`.

The fields of the table are:

- `linedefined`: as for `debug.getinfo`
- `lastlinedefined`: as for `debug.getinfo`
- `params`: the number of parameters the function takes
- `stackslots`: the number of stack slots the function's local variable use
- `upvalues`: the number of upvalues the function uses
- `bytecodes`: the number of bytecodes it the compiled function
- `gcconsts`: ??
- `nconsts`: ??
- `currentline`: as for `debug.getinfo`
- `isvararg`: if the function is a vararg function`
- `source`: as for `debug.getinfo`
- `loc`: a string describing the source and currentline, like "&lt;source&gt;:&lt;line&gt;"
- `ffid`: the fast function id of the function (if it is one).  In this case only `upvalues` above and `addr` below are valid
- `addr`: the address of the function (if it is not a Lua function).  If it's a C function rather than a fast function, only `upvalues` above is valid

