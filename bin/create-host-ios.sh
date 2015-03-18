#!/bin/bash

cd `dirname $0`/..
mkdir hosts

cd util
moaiutil host-ios -o ../hosts/ios/ $@
