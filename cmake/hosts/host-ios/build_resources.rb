#create a CMakeLists.txt with our src folder contents inside
require 'fileutils'
src = ARGV.first


files = Dir.glob(File.join(src,'*'))


cmake_content = %Q[
cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-project-res )

set ( PROJECT_RESOURCES
     #{files.map {|f| '"'+f+'"' } * "\n"}
     PARENT_SCOPE
     )
]
#files.map {|f| '"'+f+'"' } * "\n"
res_dir = 'bundle_res'
FileUtils.mkdir_p(res_dir)
File.open(File.join(res_dir,'CMakeLists.txt'),'w+') do |f|
   f.write cmake_content
end