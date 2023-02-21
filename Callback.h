#pragma once
#include <functional>

struct Callback {
	template <typename Function, typename... Args>
	Callback(Function& func, Args&&... args)
	{
		auto f = std::bind(std::forward<Function>(func), std::forward<Args>(args)...);
		func_ = [f] {f(); };
	}
	void evaluate() { func_(); }
	std::function<void()> func_;
};