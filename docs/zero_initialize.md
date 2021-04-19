## Zero Initialize

### 背景

写完自动登录任务(`login`)后，试着在`release`模式下编译，运行后发现会导致屏幕变黑，但是同样的代码在`debug`模式下却没有问题。排查问题后定位到是这段代码引起的问题：

```c++
std::array<INPUT, 2> inputs;

inputs[0].type = INPUT_MOUSE;
inputs[0].mi.dx = 0;
inputs[0].mi.dy = 0;
inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

inputs[1].type = INPUT_MOUSE;
inputs[1].mi.dx = 0;
inputs[1].mi.dy = 0;
inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
```

### 调查

以上这段代码就做了一件事：模拟鼠标的一次左键点击。在 stackoverflow 上，找到了遇到[同样问题](https://stackoverflow.com/questions/17315672/screen-goes-black-when-i-use-sendinput-to-send-mouse-cursor-positions)的老哥。在评论中，有位大神一针见血地指出了问题：

> There's no obvious connection between generating mouse input and "screen goes black". There is a flaw in your code, **you don't initialize the INPUT structure completely, generating bogus data for MOUSEINPUT.time and .dwExtraInfo.** Use `INPUT input = {};" Ensure there's a reasonable delay after you call this code.

测试后发现，确实是这样！清零后一切就都正常了～再回头看了一遍 msdn 中[相关的文档](https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-mouseinput)，似乎确实有提到有些字段最好是设置为 0，但也没有特别强调，可能当时也就忽略了。

对于解决方法，使用`INPUT input = {}`，对数组显然不适合，元素数量一多，就显得很愚蠢。经过一些调查后，发现有以下几种方法：

1.  win32 api 中有个`ZeroMemory`的宏，还有一个[`SecureZeroMemory`](<https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/aa366877(v=vs.85)>)，能无视编译器优化确保清零。

```c++
void ZeroMemory(
  // A pointer to the starting address of the block of memory to fill with zeros.
  [in] PVOID  Destination,
  // The size of the block of memory to fill with zeros, in bytes.
  [in] SIZE_T Length
);

// usage:
std::array<INPUT, 2> inputs;
ZeroMemory(inputs.data(), inputs.size() * sizeof(INPUT));
```

2.  `C`标准库中的[`memset`](https://en.cppreference.com/w/cpp/string/byte/memset)

```c++
#include <cstring>

std::array<INPUT, 2> inputs;
std::memset(inputs.data(), 0, inputs.size() * sizeof(INPUT));
```

3.  `C++`标准库`array`中的[`fill`](https://en.cppreference.com/w/cpp/container/array/fill)方法

```c++
std::array<INPUT, 2> inputs;
inputs.fill({});
```

个人更倾向第三种，毕竟最`C++`。

### 总结

1. msdn 永远是 `win32 api` 的第一手资料，不应该忽略其中的每一个细节。
2. 一定要把 struct 或 class 的初始值都定义好，留有`undefined`的变量，基本就是自己给自己挖坑了。
