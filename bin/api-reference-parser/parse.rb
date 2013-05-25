#
# Parser for Moai SDK API Reference.
# by Francisco Tufró < francisco@ziplinegames.com >
#
# Usage:
# $ ruby parse.rb [path_to_moai_source] [output_path]
#
VERSION= 0.1

require "rubygems"
require "bundler/setup"

require_relative "lib/moai_api_parser"

#
# Instructions
if ARGV.size < 2
  puts ""
  puts " Parser for Moai SDK API Reference."
  puts " #{VERSION} by Francisco Tufro < francisco@ziplinegames.com >"
  puts ""
  puts " Usage:"
  puts " $ ruby parse.rb [path_to_moai_source] [output_path]"
  puts ""
  exit 1
end

p = MoaiAPIParser.new( ARGV[0], ARGV[1] )
p.parse