#pragma once

#include <windows.h>

namespace AntiDebugger {
	// �����ڿ� ���ؼ� ������ �� -1 ���� �Ҵ� (Default)
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


