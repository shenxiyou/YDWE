#include "storm.h"
#include <aero/function/fp_call.hpp>


namespace ydwe { namespace warcraft3 {

	storm::storm() 
		: mod_(::LoadLibraryW(L"Storm.dll"))
		, valid_(load())
	{ }

	storm::~storm()
	{
		if (mod_)
		{
			::FreeLibrary(mod_);
			mod_ = NULL;
		}
	}

	bool storm::load()
	{
		if (!mod_) return false;
		if (!(fn_sfile_load_file_   = get_proc(281))) return false; 
		if (!(fn_sfile_unload_file_ = get_proc(280))) return false; 

		return true;
	}

	bool storm::valid() const
	{
		return valid_;
	}

	bool storm::load_file(const char* file_name, const void** buffer_ptr, size_t* size_ptr)
	{
		return load_file(NULL, file_name, buffer_ptr, size_ptr, 0, 1, NULL);
	}

	bool storm::load_file(HANDLE archive_handle, const char* file_name, const void** buffer_ptr, size_t* size_ptr, size_t reserve_size, uint32_t search_scope, OVERLAPPED* overlapped_ptr)
	{
		if (!valid()) return false;
		return !!aero::std_call<BOOL>(fn_sfile_load_file_, archive_handle, file_name, buffer_ptr, size_ptr, reserve_size, search_scope, overlapped_ptr);
	}

	bool storm::unload_file(const void* buffer)
	{
		if (!valid()) return false;
		return !!aero::std_call<BOOL>(fn_sfile_unload_file_, buffer);
	}

	uintptr_t storm::get_proc(uint32_t ord)
	{
		return (uintptr_t)::GetProcAddress(mod_, (const char*)ord);
	}
}}
