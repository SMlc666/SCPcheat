#include "hook/hook.hpp"
#include <atomic>
#include <gtest/gtest.h>

using namespace zr;

int __stdcall target_func(int a, int b) { return a + b; }

std::atomic<bool> hook_called{false};
int __stdcall my_hook(int a, int b) {
  hook_called = true;

  return a * b;
}

std::atomic<bool> hook2_called{false};
int __stdcall my_hook2(int a, int b) {
  hook2_called = true;
  return a - b;
}

TEST(HookTest, BasicHookInstallAndCall) {
  hook_called = false;

  Hook h(&target_func, &my_hook);

  int result = target_func(2, 3);
  EXPECT_TRUE(hook_called);
  EXPECT_EQ(result, 6);

  int orig_result = h.stdcall<int, int, int>(2, 3);
  EXPECT_EQ(orig_result, 5);
}

TEST(HookTest, UninstallRestoresOriginal) {
  hook_called = false;
  {
    Hook h(&target_func, &my_hook);
    int result = target_func(1, 2);
    EXPECT_TRUE(hook_called);
    EXPECT_EQ(result, 2);
  }

  hook_called = false;
  int result = target_func(1, 2);
  EXPECT_FALSE(hook_called);
  EXPECT_EQ(result, 3);
}
TEST(HookTest, MultiHook_SameFunction) {
  // 初始状态，调用原始函数
  hook_called = false;
  hook2_called = false;
  EXPECT_EQ(target_func(10, 3), 13);
  EXPECT_FALSE(hook_called);
  EXPECT_FALSE(hook2_called);

  // 第一次 hook
  {
    Hook h1(&target_func, &my_hook);
    hook_called = false;
    hook2_called = false;
    EXPECT_EQ(target_func(10, 3), 30); // my_hook: 10*3
    EXPECT_TRUE(hook_called);
    EXPECT_FALSE(hook2_called);

    // 第二次 hook（覆盖）
    {
      Hook h2(&target_func, &my_hook2);
      hook_called = false;
      hook2_called = false;
      EXPECT_EQ(target_func(10, 3), 7); // my_hook2: 10-3
      EXPECT_FALSE(hook_called);
      EXPECT_TRUE(hook2_called);
    }
    // 卸载 h2，h1 仍在，应该回到 my_hook
    hook_called = false;
    hook2_called = false;
    EXPECT_EQ(target_func(10, 3), 30); // my_hook: 10*3
    EXPECT_TRUE(hook_called);
    EXPECT_FALSE(hook2_called);
  }
  // 全部卸载，回到原始
  hook_called = false;
  hook2_called = false;
  EXPECT_EQ(target_func(10, 3), 13);
  EXPECT_FALSE(hook_called);
  EXPECT_FALSE(hook2_called);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
