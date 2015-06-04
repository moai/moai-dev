#include <moai-android/MOAIJString.h>

extern JavaVM* jvm;

MOAIJString::~MOAIJString () {

	if ( this->mJString ) {

		JNI_GET_ENV(jvm, env);
		env->DeleteLocalRef ( this->mJString );
	}
}