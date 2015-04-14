#!/bin/bash

cd `dirname $0`/..
mkdir hosts

cd util
moaiutil host-osx-app -o ../hosts/osx/ $@
