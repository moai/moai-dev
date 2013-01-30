// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "gtest/gtest.h"
#include <aku/AKU.h>
#include <lua-headers/moai_lua.h>
#include "moaicore/moaicore.h"

TEST(MOAILuaState, CanBeCreated) {
  MOAILuaState* o = new MOAILuaState ();
  EXPECT_TRUE(o);
}

TEST(MOAILuaState, StateIsZeroByDefault) {
  MOAILuaState* o = new MOAILuaState ();
  EXPECT_EQ(0, o->mState);
}