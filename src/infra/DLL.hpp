#pragma once

#include "infra/Log.hpp"
#include <Windows.h>
#include <string>

class DLL {
	HMODULE m_module;

public:
	DLL(const std::string &moduleName, bool useSystemDir = true);
	~DLL();
	bool isModuleFound() const;

	template <typename FuncPtr> FuncPtr getFunc(const std::string &funcName) {
		AutomataMod::log(AutomataMod::LogLevel::LOG_INFO, "Hooking {}", funcName);
		if (!m_module) {
			return nullptr;
		}

		FuncPtr func = (FuncPtr)GetProcAddress(m_module, funcName.c_str());
		if (!func) {
			AutomataMod::log(AutomataMod::LogLevel::LOG_ERROR, "Failed to hook {} Error code: {}", funcName, GetLastError());
			return nullptr;
		}

		AutomataMod::log(AutomataMod::LogLevel::LOG_INFO, "Hooked {}", funcName);
		return func;
	}
};
