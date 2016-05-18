require "lfs"


for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
  
  if param then

    print()

  end
end
upload = "lua-docs-html"
os.execute("s3cmd rm --recursive s3://moai-downloads/"..upload )
os.execute("s3cmd put --recursive ../"..upload .." s3://moai-downloads/" )
