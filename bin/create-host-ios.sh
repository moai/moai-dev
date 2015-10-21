#!/bin/bash

cd `dirname $0`/..
mkdir hosts

cd util
pito host-ios -o ../hosts/ios/ $@
