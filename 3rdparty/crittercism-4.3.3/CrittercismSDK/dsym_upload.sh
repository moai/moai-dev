#!/bin/sh
# Copyright 2013 Crittercism, Inc. All rights reserved.
#
# Usage:
#   * (These instructions assume you have dragged the "CrittercismSDK" folder
#     into your project in XCode)
#   * In the project editor, select your target.
#   * Click "Build Phases" at the top of the project editor.
#   * Click "Add Build Phase" in the lower right corner.
#   * Choose "Add Run Script."
#   * Paste the following script into the dark text box. You will have to 
#     uncomment the lines (remove the #s) of course.
#
# --- SCRIPT BEGINS ON NEXT LINE, COPY AND EDIT FROM THERE ---
# APP_ID="<YOUR_APP_ID>"
# API_KEY="<YOUR_API_KEY>"
# source ${SRCROOT}/CrittercismSDK/dsym_upload.sh
# --- END OF SCRIPT ---

################################################################################
# Advanced Settings
#
# You can over-ride these directly in XCode in your Run Script Build Phase, or
# change the defaults below.

# Should simulator builds cause symbols to be uploaded?
UPLOAD_SIMULATOR_SYMBOLS=${UPLOAD_SIMULATOR_SYMBOLS:=1}

# This setting determines whether or not your build will fail if the dSYM was
# not uploaded properly to Crittercism's servers.
#
# You may wish to change this setting if you are building without internet
# access, or otherwise are having difficulty connecting to Crittercism's
# servers.
REQUIRE_UPLOAD_SUCCESS=${REQUIRE_UPLOAD_SUCCESS:=1}

################################################################################
# You should not need to edit anything past this point.

DSYM_UPLOAD_ENDPOINT="https://api.crittercism.com/api_beta/dsym/"
exitWithMessageAndCode() {
  echo "${1}"
  exit ${2}
}

echo "Uploading dSYM to Crittercism"
echo ""

# Check to make sure the necessary parameters are defined
if [ ! "${APP_ID}" ]; then
  exitWithMessageAndCode "err: Crittercism App ID not defined." 1
fi

if [ ! "${API_KEY}" ]; then
  exitWithMessageAndCode "err: Crittercism API Key not defined." 1
fi

# Display build info
BUNDLE_VERSION=$(/usr/libexec/PlistBuddy -c 'Print CFBundleVersion' ${INFOPLIST_FILE})
BUNDLE_SHORT_VERSION=$(/usr/libexec/PlistBuddy -c 'Print CFBundleShortVersionString' ${INFOPLIST_FILE})

echo "Product Name: ${PRODUCT_NAME}"
echo "Version: ${BUNDLE_SHORT_VERSION}"
echo "Build: ${BUNDLE_VERSION}"
echo "Crittercism App ID: ${APP_ID}"
echo "Crittercism API key: ${API_KEY}"

# Possibly bail if this is a simulator build
if [ "$EFFECTIVE_PLATFORM_NAME" == "-iphonesimulator" ]; then
  if [ $UPLOAD_SIMULATOR_SYMBOLS -eq 0 ]; then
  	exitWithMessageAndCode "skipping simulator build" 0
  fi
fi

DSYM_SRC=${DWARF_DSYM_FOLDER_PATH}/${DWARF_DSYM_FILE_NAME}
DSYM_ZIP_FPATH="/tmp/$DWARF_DSYM_FILE_NAME.zip"

# create dSYM .zip file
echo "dSYM location: ${DSYM_SRC}"
if [ ! -d "$DSYM_SRC" ]; then
  exitWithMessageAndCode "dSYM not found: ${DSYM_SRC}" 1
fi

echo "compressing dSYM to: ${DSYM_ZIP_FPATH} ..."
(/usr/bin/zip --recurse-paths --quiet "${DSYM_ZIP_FPATH}" "${DSYM_SRC}") || exitWithMessageAndCode "err: failed creating zip" 1
echo ""
echo "Archive created."

# Upload dSYM to Crittercism
URL=${DSYM_UPLOAD_ENDPOINT}${APP_ID}
echo "Uploading dSYM to Crittercism: ${URL}"

#curl "${URL}" --write-out %{http_code} --silent --output /dev/null -F dsym=@"${DSYM_ZIP_FPATH}" -F key="${API_KEY}" 2>&1
STATUS=$(curl "${URL}" --write-out %{http_code} --silent --output /dev/null -F dsym=@"${DSYM_ZIP_FPATH}" -F key="${API_KEY}")

echo Crittercism API server response: ${STATUS}
if [ $STATUS -ne 200 ]; then
  if [ $REQUIRE_UPLOAD_SUCCESS -eq 1 ]; then
  	echo "err: dSYM archive not succesfully uploaded."
  	echo "To ignore this condition and build succesfully, add:"
  	echo "REQUIRE_UPLOAD_SUCCESS=0"
  	echo "to the Run Script Build Phase invoking this script."
    exit 1
  else
  	echo "err: dSYM archive not succesfully uploaded"
  	echo "ignoring due to REQUIRE_UPLOAD_SUCCESS=0"
  fi
fi


# Remove temp dSYM archive
echo "Cleaning up temporary dSYM archive..."
/bin/rm -f "${DSYM_ZIP_FPATH}"

echo "Crittercism dSYM upload complete."

if [ "$?" -ne 0 ]; then
  exitWithMessageAndCode "an error was encountered uploading dSYM" 1
fi
