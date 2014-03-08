#include <gtest/gtest.h>
#include <dragon.h>
#include "config.h"

using namespace dragon;

TEST(core, parseConfig1) {
  JavaScriptCompiler jsc;
  Config config(jsc);
  config.Parse(DRAGON_TEST_PATH"testParseConfig/App.conf");

  ASSERT_STREQ("12", config["test"]["mysql"].c_str());
  ASSERT_STREQ("", config["test"]["pass"].c_str());
  ASSERT_STREQ("\"123\"", config["test"]["name"].c_str());
}
