#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>

/**
 * Provides utility methods for dealing with the JNI.
 */
class JniUtils {
public:
    static jobjectArray arrayFromLua ( lua_State* L, int index );
    static jobject bundleFromLua ( lua_State* L, int index );
    static jobject createObjectOfClass ( STLString type );
    static jmethodID getMethod ( STLString className, STLString methodName, STLString methodSignature );
    static cc8* parseLuaTable ( lua_State* L, int idx );
};
