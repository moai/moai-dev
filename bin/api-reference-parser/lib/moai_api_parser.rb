require 'ruby-progressbar'
require 'haml'
require 'nokogiri'
require 'fileutils'

class MoaiAPIParser

  #
  # source should be a string indicating the directory
  # where all the source files are located.
  attr_accessor :source, :version_major, :version_revision, :version_author

  #
  # destination should be a string indicating the directory
  # where the generated documentation should go to.
  attr_accessor :destination

  def initialize ( source, destination )

    #
    # Check if source directory exists and it's actually
    # a directory.
    if !(File.exists?(source) and File.directory?(source))
      # Raise an error and exit if that's the case
      raise "Source directory doesn't exist"
      exit 1
    end

    self.source = source
    self.destination = destination

  end

  #
  # this will make all the magic happen
  def parse
    copy_source
    parse_with_lua
    parse_version
    parse_with_doxygen
    inject_intro_page
    cleanup_doxygen_html
    cleanup_doxygen_js
    cleanup_extra_files
 end

  #
  # This method copies our source tree to a temporary directory
  def copy_source
    FileUtils.makedirs(destination)
    FileUtils.cp_r source, destination
    self.source = destination + "src"

    # Remove the files that are not from MOAI
    # This includes:
    #  * FMOD Files ( using /Source/ as the pattern to match )
    #  * OpenGLContext from NaCL
    Dir.glob(destination + 'src/**/*').each { |f| 
      if !File.directory?(f) and (( f =~ /\/Source\// ) or ( f =~ /opengl_context\.h/))
        File.delete(f) 
      end
    }

  end

  #
  # We use some custom tags to annotate our source code.
  # In order for that to be parsable by doxygen we need to 
  # run a couple of replacements.
  def parse_with_lua
    ['h', 'cpp', 'mm'].each do |extension|
      parse_with_lua_for(extension)
    end
  end

  #
  # We have lua-based parsers that search and replace
  # our code for specific stuff using MOAIParser.
  # This method searchs for all the files for a specific
  # extension and then runs the replacements using 
  # the system's moai executable
  def parse_with_lua_for(extension)
    
    #
    # Our parsers expect a file called
    # input.[extension] and return a file called output.[extension]
    # so we need to cleanup everything just in case.
    if File.exists?("input.#{extension}")
      FileUtils.rm ("input.#{extension}")
    end
    if File.exists?("output.#{extension}")
      FileUtils.rm ("output.#{extension}")
    end

    # Since we have different scripts for each different
    # file type, we have to decide which script to run...
    parser_script = File.dirname(__FILE__) + "/../lua-parsers/"
    case extension
    when "h"
      parser_script += "header-files.lua"
    when "cpp"
      parser_script += "source-files-cpp.lua"
    when "mm"
      parser_script += "source-files-mm.lua"
    end

    # Now we iterate through all these files and
    # parse them.
    files = all_source_files(source, extension)
    pb = ProgressBar.create(:title => "Fixing #{extension}", :total => files.size)
    files.each do |file|
      FileUtils.cp file, "input.#{extension}"
      system "moai #{parser_script} >/dev/null"

      # The moai parser outputs a file called output.[extension]
      # so we want to copy it back.
      FileUtils.mv "output.#{extension}", file
      FileUtils.rm "input.#{extension}"
      pb.increment
    end

  end

  def parse_version
    file = File.open ( destination + "src/config-default/moai_version.h" )
    data = file.read

    data.match ( /MOAI_SDK_VERSION_MAJOR_MINOR\s(\S*)\s*/ )
    self.version_major = $1

    data.match ( /MOAI_SDK_VERSION_REVISION\s(\S*)\s*/ )
    self.version_revision = $1

    data.match ( /MOAI_SDK_VERSION_AUTHOR\s\"(.*)\"\s*/ )
    self.version_author = $1    

  end

  def full_version
    v = "#{version_major}"
    
    if version_revision.to_i > 0
      v += " revision #{version_revision}" 
      v += " (#{version_author})" if version_author.length > 0
    else
      v += " (ad hoc build by #{version_author})" if version_author.length > 0
    end
    v
  end

  def parse_with_doxygen
    configure_doxygen
    system ( "doxygen #{destination}moai-api-doxygen.cfg" )
  end

  def configure_doxygen
    # Copy default configuration file to our destination
    # directory.
    FileUtils.cp(File.dirname(__FILE__) + '/../moai-api-doxygen.cfg', destination + "moai-api-doxygen.cfg")

    # Replace the output and input directories with the correct ones
    file = File.open(destination + 'moai-api-doxygen.cfg', 'r')
    config = file.read

    config.gsub!(/@@OUTPUT_DIRECTORY@@/, destination)
    config.gsub!(/@@INPUT_DIRECTORY@@/, source)
    config.gsub!(/@@VERSION@@/, "#{full_version}")

    File.open(destination + 'moai-api-doxygen.cfg', 'w') do |file|
      file.write config
    end
  end

  #
  # We're using a customized intro page that gets injected into
  # doxygen's index.html through this method.
  # To modify the intro page, just change the intro_page.haml on
  # the root directory of this library.
  # It's written using http://haml.info/
  def inject_intro_page

    # Open the haml file and get the html version
    file = File.open(File.dirname(__FILE__) + "/../intro_page.haml", "r")
    haml_string = file.read
    html_string = Haml::Engine.new(haml_string).render
    file.close

    # Open doxygen's index.html and inject the generated html
    file = File.open(destination + "html/index.html", "r")
    index_string = file.read
    index_string.gsub!(/<div class="contents">\s+<\/div><!-- contents -->/, 
      "<div class=\"contents\">\n#{html_string}<\/div><!-- contents -->")
    file.close

    # write the result string into index.html
    file = File.open(destination + "html/index.html", "w") do |file|
      file.write ( index_string )
    end
  end

  # There are some other replacements that are needed
  # for our final version of the docs. 
  # These are made using gsub and a set of predefined regexps
  HTML_REPLACEMENTS = [
    { :regexp => /Static .*? Member Functions/, :replace_with => "Function List" },
    { :regexp => /Member Function Documentation/, :replace_with => "Function Documentation" },
    { :regexp => /static int(?<link>.*?\"\>?)_(?<method>.*?\<\/a\>?)/, :replace_with => "\\k<link>\\k<method>" },
    { :regexp => /\(lua_State \*L\)/, :replace_with => "" },
    { :regexp => /\>lua_State \*.*?\</, :replace_with => "><" },
    { :regexp => /int \<(?<link>.*?)MOAI.*?::.*_(?<method>.*?)\<\/a\>/, :replace_with => "<\\k<link>\\k<method></a>" },
    { :regexp => /\<em\>L\<\/em\>/, :replace_with => "" },
    { :regexp => /\<td class=\"paramtype\"\>\<\/td\>/, :replace_with => "" },
    { :regexp => /\<td class=\"paramname\"\>\<\/td\>/, :replace_with => "" },
    { :regexp => /\<td\>\(\<\/td\>/, :replace_with => "" },
    { :regexp => /\<td\>\)\<\/td\>/, :replace_with => "" },
    { :regexp => /\[static, private\]/, :replace_with => "" },
    { :regexp => /\[static, protected\]/, :replace_with => "" },
    { :regexp => /\[private, static\]/, :replace_with => "" },
    { :regexp => /\[protected, static\]/, :replace_with => "" },
    { :regexp => /\<li\>_(?<method>.*?)\n/, :replace_with => "<li>\\k<method>" },

    # Methods in -members.html files
    { :regexp => /\<td\>\<a(?<link>.+?)\"\>_(?<method>.+?)\<\/a\>\<\/td\>/, :replace_with => "<td><a\\k<link>\">\\k<method></a></td>\n"},

    #
    # Doxytags
    { :regexp => /\<\!\-\-\sdoxytag:\smember="(?<class>.+?)_(?<method>.+?)\"/, :replace_with => "<!-- doxytag: member=\"\\k<class>\\k<method>\""},
    
  ] 


  def cleanup_doxygen_html
    files = all_source_files( destination + "html", "html")
    pb = ProgressBar.create(:title => "Cleanup HTML", :total => files.size)
    files.each do |file_name|
      # Get the file's content into a string
      file = File.open( file_name, 'r' )
      html_content = file.read
      file.close

      # Do the replacements
      HTML_REPLACEMENTS.each do |replacement|
        html_content.gsub!(replacement[:regexp], replacement[:replace_with])
      end

      # Some replacements that are easier with Nokogiri because
      # are DOM related.
      html_content = cleanup_with_nokogiri( html_content )

      # Write the replaced files back
      File.open( file_name, 'w' ) do |file|
        file.write( html_content )
      end
      pb.increment
    end
  end

  #
  # Doxygen generates some JavaScript files used
  # to create the tree views and searchs.
  # We need to remove all the C++ methods from these
  # JavaScripts and remove the underscores from the 
  # Lua method names.
  #
  def cleanup_doxygen_js
    files = all_source_files( destination + "html", "js")
    pb = ProgressBar.create(:title => "Cleanup JS", :total => files.size)
    
    files.each do |file_name|
      # Only parse files from MOAI classes
      if file_name =~ /m_o_a_i_/
        js_content = ""

        # Get the file's content into a string
        file = File.open( file_name, 'r' ).each do |line|

          # If it's a method
          if line =~ /\s+\[/
            # If the method starts with _ then remove the underscore and add it to the source
            # file. If not, avoid adding it, since it's a C++ method.
            if line =~ /\s+\[\s\"_/
              js_content << line.gsub(/(?<spaces>\s+?)\[\s\"_(?<method>.+?)\"/, 
                "\\k<spaces>[ \"\\k<method>\"")
            end
          else
            js_content << line
          end
        end

        # Write the replaced files back
        File.open( file_name, 'w' ) do |file|
          file.write( js_content )
        end

      end
      pb.increment
    end
  end

  def cleanup_with_nokogiri(text)
    #
    # Parse the file
    html = Nokogiri::HTML(text)
    
    # 
    # Remove all the methods that don't have _
    #navhtml.css("div#navrow1").remove

    return html.to_html
  end


  #
  # During this script we create som adhoc files.
  # This method removes them
  def cleanup_extra_files
    FileUtils.rm( destination + "moai-api-doxygen.cfg" )
    FileUtils.rm_r( destination + "src" )
  end


  def all_source_files( directory, extension )
    files = []

    Dir.foreach ( directory ) do |file|

      # we have to exclude . and ..
      if !['..', '.'].include?(file)

        full_path = directory + "/" + file

        # do a recursion if current file is a directory
        if File.directory?(full_path)
          files.concat( all_source_files( full_path, extension ) )
        else
          # avoid the files that are not from the given extension
          # avoid also the classes that are not called MOAI*
          if full_path =~ /\.#{extension}$/
            files << full_path
          end
        end
      end

    end
    return files
  end

end

#
# THE FOLLOWING CODE IS FOR A FUTURE VERSION OF THIS PARSER.
# WE MAY WANT TO REPLACE LUA PARSERS AND THIS IS SOME GROUND WORK FOR THAT
#

# # General cases
# IN_NO_DESC =    /@in\s+(?<type>\S+?)\s+(?<name>\S+?)\n/
# IN_WITH_DESC =  /@in\s+(?<type>\S+?)\s+(?<name>\S+?)\s+(?<description>.+?)\n/

# OUT_NO_DESC =   /@out\s+(?<type>\S+?)\s+(?<name>\S+?)\n/
# OUT_WITH_DESC = /@out\s+(?<type>\S+?)\s+(?<name>\S+?)\s+(?<description>.+?)\n/  

# OPT_NO_DESC =   /@opt\s+(?<type>\S+?)\s+(?<name>\S+?)\n/
# OPT_WITH_DESC = /@opt\s+(?<type>\S+?)\s+(?<name>\S+?)\s+(?<description>.+?)\n/

# # TODO: Overloaded
# OVERLOAD_REGEXP = /@overload(?<description>.+??)\n/

# # Special cases
# SELF_REGEX = //
# NILOUT_REGEX = /@out\s+nil\n/


# def replace_source
#   # For each source file
#   all_source_files( source ).each do |source_file|
#     # open it
#     file = File.open(source_file, "r")
#     source_code = file.read
    
#     # run replacements
#     source_code = replace_parameters ( source_code )

#     # close it
#     file.close

#     # rewrite
#     File.open(source_file, 'w') { |file| file.write(source_code) }
    
#   end
  
# end

#
# @in, @out, @opt and @overload should be replaced
# def replace_parameters ( string )

#   # First we run the special cases
#   new_string = string.gsub(NILOUT_REGEX, "@param[out] nil <type>nil</type>\n")

#   # Then the general ones
#   new_string.gsub!(IN_NO_DESC,    "@param[in] \\k<name> <type>\\k<type></type>\n")
#   new_string.gsub!(IN_WITH_DESC,  "@param[in] \\k<name> <type>\\k<type></type> <description>\\k<description></description>\n")
  
#   new_string.gsub!(OUT_NO_DESC,   "@param[out] \\k<name> <type>\\k<type></type>\n")
#   new_string.gsub!(OUT_WITH_DESC, "@param[out] \\k<name> <type>\\k<type></type> <description>\\k<description></description>\n")

#   new_string.gsub!(OPT_NO_DESC,   "@param[in] \\k<name> <type>\\k<type></type> <optional>Optional</optional>\n")
#   new_string.gsub!(OPT_WITH_DESC, "@param[in] \\k<name> <type>\\k<type></type> <optional>Optional</optional> <description>\\k<description></description>\n")

#   new_string
# end
