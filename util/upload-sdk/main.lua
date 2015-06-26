require "lfs"

for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
	
	if param then

		print()

	end
end

directorypath = "./" -- current working directory
upload = ""
for filename in lfs.dir(directorypath) do
    if filename:match("%.zip$") then -- "%." is an escaped ".", "$" is end of string
        upload = filename
    end
end
os.execute("s3cmd put "..upload .." s3://moai-downloads/develop/"..upload )