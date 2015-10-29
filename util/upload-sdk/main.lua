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

os.execute("s3cmd ls s3://moai-downloads/develop/ > output.txt")

result = io.open(directorypath .. "output.txt")
local t = {}
while true do
	local line = result:read("*line")
	if(line == nil) then break end
	local info = {}
	for x in string.gmatch(line,"%S+") do
		table.insert(info,x)
	end
	table.insert(t,info)
end
local file = io.open("moai-downloads.html","w")
function Beginhtml(  )
  file:write([[
<div class="container" style="font-family:arial">
  <h2>Moai SDK</h2>
  <p>Most-recent builds</p>            
  <table class="table table-hover" width="100%">
    <thead>
      <tr>
        <th>File name</th>
        <th>Build number</th>
        <th>Date</th>
      </tr>
    </thead>
    <tbody>
]])
end

function midhtml(  )
  file:write([[
    </tbody>
  </table>
  <p>Previous builds</p>
  <table class="table table-hover">
    <thead>
      <tr>
        <th>File name</th>
        <th>Build number</th>
        <th>Date</th>
      </tr>
    </thead>
    <tbody>
]])
end
function endhtml( )
  file:write([[
    </tbody>
  </table>
</div>
]])
end

function writebinaryinfo( T )
  local link = string.gsub(T[4],"s3://","https://s3-us-west-1.amazonaws.com/")
  startname = string.find(T[4],"/[^/]*$")
  endname = string.len(T[4])
  start,endchar = string.find(T[4],"%d+%.%d+%.%d+")
  
  file:write([[
      <tr>
        <td><a href="]]
        ..link..[[">]]..string.sub(T[4],startname+1,endname)..[[</a></td>
        <td>]]..string.sub(T[4],start,endchar)..[[</td>
        <td>]]..T[1]..[[</td>
      </tr>
]])
end

function cmp_date(op1, op2)
    local date1, date2 = op1[1], op2[1]
    return date1<date2
    
end


table.sort( t ,cmp_date)
print(t[#t][4])
start,endchar = string.find(t[#t][4],"%d+%.%d+%.%d+")
print(string.sub(t[#t][4],start,endchar))

for index = #t,2,-1 do
	print(t[index][4])
end
Beginhtml()
writebinaryinfo(t[#t])
midhtml()
for index = #t-1,2,-1 do
  writebinaryinfo(t[index])
end
endhtml()
file:close()
os.execute("s3cmd put ".."moai-downloads.html" .." s3://moai-downloads/".."moai-downloads.html" )