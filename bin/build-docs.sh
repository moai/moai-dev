#!/bin/bash
#
#
RUBY=`which ruby`
BUNDLE=`which bundle`
DOXYGEN=`which doxygen`
MOAI=`which moai`
DOT=`which dot`

if [[ -z $RUBY ]]; then
  echo "You need to have Ruby 1.9+ installed. Check http://www.ruby-lang.org/";
  exit 1;
fi

if [[ -z $BUNDLE ]]; then
  echo "You need to have bundler installed. Check http://gembundler.com/"
  exit 1
fi

if [[ -z $DOXYGEN ]]; then
  echo "You need to have Doxygen installed.";
  exit 1;
fi

if [[ -z $DOT ]]; then
  echo "You need to have graphviz (and dot command on PATH) installed.";
  exit 1;
fi

if [[ -z $MOAI ]]; then
  echo "You need to have moai in your PATH.";
  exit 1;
fi

rm -rf docs/api-reference
cd `dirname $0`/..
cd bin/api-reference-parser
bundle install --path vendor/bundle
ruby parse.rb ../../src/ ../../release/docs/api-reference/
if [[ $? -ne 0 ]]; then
    exit 1
fi
cd ../../