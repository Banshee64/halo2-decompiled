// @flags /O1 /Gr
/* UNKNOWN_23068B.CPP: the screens' create function getters (slot 26 of the
   screen vtables), one class per vtable until the screens are written */

#include "cseries.h"
#include "screen_widgets.h"

c_screen_widget *__stdcall function_230616(s_screen_parameters *request);
c_screen_widget *__stdcall function_230691(s_screen_parameters *request);
c_screen_widget *__stdcall function_2310b7(s_screen_parameters *request);
c_screen_widget *__stdcall function_2312c2(s_screen_parameters *request);
c_screen_widget *__stdcall function_2313a8(s_screen_parameters *request);
c_screen_widget *__stdcall function_231995(s_screen_parameters *request);
c_screen_widget *__stdcall function_2320c4(s_screen_parameters *request);
c_screen_widget *__stdcall function_231db5(s_screen_parameters *request);
c_screen_widget *__stdcall function_23252e(s_screen_parameters *request);
c_screen_widget *__stdcall function_2325fb(s_screen_parameters *request);
c_screen_widget *__stdcall function_2323c3(s_screen_parameters *request);
c_screen_widget *__stdcall function_23246a(s_screen_parameters *request);
c_screen_widget *__stdcall function_23334f(s_screen_parameters *request);
c_screen_widget *__stdcall function_23764f(s_screen_parameters *request);
c_screen_widget *__stdcall function_23784f(s_screen_parameters *request);
c_screen_widget *__stdcall function_237713(s_screen_parameters *request);
c_screen_widget *__stdcall function_2312af(s_screen_parameters *request);

class c_screen_458a00 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_458ac8 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_458ba0 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_458d08 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_458de8 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_458e58 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_458fa8 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

/* c_screen_4590b8 is in screen_widgets.h; 0x459148 shares its slot 10 */
class c_screen_459148 : public c_screen_4590b8
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_4591b8 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_459228 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_459338 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_4596e0 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_459ae8 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_459ba0 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_459c10 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_458c98 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x23068b
screen_load_proc c_screen_458a00::get_load_proc()
{
	return function_230616;
}

// @retail 0x230714
screen_load_proc c_screen_458ac8::get_load_proc()
{
	return function_230691;
}

// @retail 0x230c87
screen_load_proc c_screen_458ba0::get_load_proc()
{
	return function_2310b7;
}

// @retail 0x231339
screen_load_proc c_screen_458d08::get_load_proc()
{
	return function_2312c2;
}

// @retail 0x23141f
screen_load_proc c_screen_458de8::get_load_proc()
{
	return function_2313a8;
}

// @retail 0x2312bc
screen_load_proc c_screen_458e58::get_load_proc()
{
	return function_231995;
}

// @retail 0x231daf
screen_load_proc c_screen_458fa8::get_load_proc()
{
	return function_2320c4;
}

// @retail 0x231e28
screen_load_proc c_screen_4590b8::get_load_proc()
{
	return function_231db5;
}

// @retail 0x2325a1
screen_load_proc c_screen_459148::get_load_proc()
{
	return function_23252e;
}

// @retail 0x23266b
screen_load_proc c_screen_4591b8::get_load_proc()
{
	return function_2325fb;
}

// @retail 0x232433
screen_load_proc c_screen_459228::get_load_proc()
{
	return function_2323c3;
}

// @retail 0x2324dd
screen_load_proc c_screen_459338::get_load_proc()
{
	return function_23246a;
}

// @retail 0x232d4e
screen_load_proc c_screen_4596e0::get_load_proc()
{
	return function_23334f;
}

// @retail 0x2376c2
screen_load_proc c_screen_459ae8::get_load_proc()
{
	return function_23764f;
}

// @retail 0x237649
screen_load_proc c_screen_459ba0::get_load_proc()
{
	return function_23784f;
}

// @retail 0x237791
screen_load_proc c_screen_459c10::get_load_proc()
{
	return function_237713;
}

// @retail 0x232d48
screen_load_proc c_screen_458c98::get_load_proc()
{
	return function_2312af;
}

/* the create function of the screens that cannot be created */
// @retail 0x2312af
c_screen_widget *__stdcall function_2312af(s_screen_parameters *request)
{
	return 0;
}
