#!/usr/bin/env bash 

PROJECT_HOME=$(cd $(dirname "${0}")/../ && pwd)
FOLDER="${PROJECT_HOME}/src/.config/"
FILENAME="${FOLDER}/moai_build.h"

mkdir -p $FOLDER

GIT_TAG=$(git describe --long --dirty --always --tags)
GIT_COMMIT=$(git rev-parse HEAD)
GIT_USER_NAME=$(git config user.name)
BUILD_DATE=$(date +"%D %T %Z")

printf "#ifndef MOAI_BUILD_H\n" > $FILENAME
printf "#define MOAI_BUILD_H\n" >> $FILENAME
printf "\n" >> $FILENAME

printf "#define MOAI_BUILD_DATE_STR \"%s\"\n" "$BUILD_DATE" >> $FILENAME
printf "#define MOAI_GIT_COMMIT_STR \"%s\"\n" "$GIT_COMMIT" >> $FILENAME
printf "#define MOAI_GIT_TAG_STR \"%s\"\n" "$GIT_TAG" >> $FILENAME
printf "#define MOAI_GIT_USER_NAME \"%s\"\n" "$GIT_USER_NAME" >> $FILENAME

printf "\n" >> $FILENAME
printf "#endif\n" >> $FILENAME
