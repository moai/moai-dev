# set up the MOAI SDK environment and toolbelt
export MOAI_SDK_HOME=`pwd`
export PATH=$MOAI_SDK_HOME/util:$PATH
echo "The MOAI SDK home is: $MOAI_SDK_HOME"
echo -n "Local MOAI host is installed? " && which moai
echo -n "moaiutil toolbelt is ready? " && which moaiutil && moaiutil sdk-version

