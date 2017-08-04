#pragma once

#include <windows.h>

namespace AntiDebugger {
	// 생성자에 의해서 생설될 때 -1 값을 할당 (Default)
	// Abstract class
	class CDebugger
	{
		public:
			CDebugger();
			~CDebugger();
			virtual DWORD CheckDebuggerPresent() = 0;
		protected:
			DWORD dwDebuggerPresent;
	};

	class CDebuggerProcessDebugFlag : public CDebugger
	{
		DWORD CheckDebuggerPresent();
	};
}


