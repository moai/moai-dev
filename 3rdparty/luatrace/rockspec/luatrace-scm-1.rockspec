package = "luatrace"
version = "scm-1"
source =
{
  url = "git://github.com/geoffleyland/luatrace.git",
  branch = "master"
}
description =
{
  summary = "Traces Lua execution for profiling and coverage analysis",
  homepage = "http://github.com/geoffleyland/luatrace",
  license = "MIT/X11",
  maintainer = "Geoff Leyland <geoff.leyland@incremental.co.nz>"
}
dependencies = { "lua >= 5.1" }
build =
{
  type = "builtin",
  modules =
  {
    luatrace = "lua/luatrace.lua",
    ["luatrace.profile"] = "lua/luatrace/profile.lua",
    ["luatrace.trace_file"] = "lua/luatrace/trace_file.lua",
    uatrace = "lua/uatrace.lua",
    ["uatrace.profile"] = "lua/uatrace/profile.lua",
    ["luatrace.c_hook"] = "c/c_hook.c",
  },
  install = { bin = { "sh/luatrace.profile" } },
}
