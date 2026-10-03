// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_2ACCD0.CPP: saved game files on the Xbox hard disk: signed reads
   and writes run as asynchronous tasks, and copying files */

#include "cseries.h"
#include <xtl.h>
#include <string.h>
#include "unknown_2accd0.h"

/* the location and type of a saved game file (0x40 bytes) */
struct s_saved_game_file_location
{
	char name[0x14];
	wchar_t unknown14[0x12];
	long type;
	long unknown3c;
};

struct s_saved_game_file_id
{
	byte data[0x14];
};

/* a saved game file (0xa0 bytes) */
struct s_saved_game_file
{
	long unknown00;
	s_saved_game_file_location location;
	wchar_t display_name[0x11];
	s_saved_game_file_id id;
	bool unknown7a;
	byte unknown7b;
	bool temporary;
	byte unknown7d[3];
	long unknown80;
	long unknown84;
	long unknown88;
	long unknown8c;
	long unknown90;
	long unknown94;
	long unknown98;
	long unknown9c;
};

/* copies a file in chunks */
struct s_file_copy
{
	byte unknown00[0x84];
	HANDLE source;
	HANDLE destination;
	void *buffer;
	dword size;
	dword read_offset;
	dword write_offset;
};

struct s_saved_game_file_read_parameters
{
	void *buffer;
	dword size;
	bool non_roamable;
	s_saved_game_file_task *task;
};

#define FILE_COPY_CHUNK_SIZE 0x20000
#define MIN(a, b) ((a) > (b) ? (b) : (a))

const char *function_216b60(long type);
char *function_122810(char *path, const char *name);

void saved_game_file_read(file_reference *file, void *buffer, dword size, bool non_roamable, s_saved_game_file_task *task);
void saved_game_file_write(file_reference *file, void *buffer, dword size, bool non_roamable, s_saved_game_file_task *task);

inline char *csstrnzcpy(char *destination, const char *source, dword size)
{
	strncpy(destination, source, size);
	destination[size - 1] = 0;
	return destination;
}

inline void file_reference_create(file_reference *file)
{
	memset(file, 0, sizeof(*file));
	file->signature = FILE_REFERENCE_SIGNATURE;
	file->location = NONE;
}

inline void file_reference_set_name(file_reference *file, const char *name)
{
	if (file->flags & 1)
	{
		file_path_remove_name(file->path);
	}
	file_path_add_name(file->path, name);
	file->flags |= 1;
}

inline void file_reference_create_from_path(file_reference *file, const char *path)
{
	file_reference_create(file);
	file_reference_set_name(file, path);
}

/* Bungie's argument order */
inline bool file_write(file_reference *file, dword size, const void *buffer)
{
	return function_136d00(file, buffer, size);
}

inline bool file_read(file_reference *file, dword size, bool silent, void *buffer)
{
	return function_136ca0(file, buffer, size, silent);
}

inline bool file_close(file_reference *file)
{
	bool success = false;
	if (CloseHandle(file->handle))
	{
		file->handle = 0;
		file->position = 0;
		success = true;
	}
	else
	{
		GetLastError();
		SetLastError(0);
	}
	return success;
}

// @retail 0x2accd0
PRIVATE long __stdcall saved_game_file_read_work(s_async_task *task, void *parameters_view, long parameters_size)
{
	s_saved_game_file_read_parameters *parameters = (s_saved_game_file_read_parameters *)parameters_view;
	file_reference file;

	parameters->task->succeeded = false;
	parameters->task->state = 4;
	file_reference_create_from_path(&file, parameters->task->path);
	saved_game_file_read(&file, parameters->buffer, parameters->size, parameters->non_roamable, parameters->task);
	return 1;
}

// @retail 0x2acd60
bool saved_game_file_read_begin(void *buffer, dword size, bool non_roamable, s_saved_game_file_task *task, const char *path)
{
	s_saved_game_file_read_parameters parameters;

	task->unknown1 = false;
	task->succeeded = false;
	task->progress = -1.0f;
	csstrnzcpy(task->path, path, sizeof(task->path));
	parameters.buffer = buffer;
	parameters.size = size;
	parameters.non_roamable = non_roamable;
	parameters.task = task;
	return async_task_add_work(saved_game_file_read_work, sizeof(parameters), &parameters, 2, &task->done) != NONE;
}

// @retail 0x2acf40
void saved_game_file_new(s_saved_game_file *file, long unknown00, const s_saved_game_file_location *location)
{
	file->unknown00 = unknown00;
	file->location.name[0] = 0;
	file->location.unknown14[0] = 0;
	file->display_name[0] = 0;
	file->id.data[0] = 0;
	file->unknown7a = false;
	file->temporary = false;
	file->unknown80 = 0;
	file->unknown8c = 0;
	file->unknown9c = 6;
	file->location = *location;
}

// @retail 0x2acf80
s_saved_game_file *saved_game_file_new_from_id(s_saved_game_file *file, long unknown00, const wchar_t *display_name, const s_saved_game_file_location *location, long unknown8c, long unknown80)
{
	file->unknown00 = unknown00;
	file->location.name[0] = 0;
	file->location.unknown14[0] = 0;
	wcsncpy(file->display_name, display_name, 0x10);
	file->display_name[0x10] = 0;
	file->id = *(const s_saved_game_file_id *)location;
	file->unknown80 = unknown80;
	file->temporary = false;
	file->unknown84 = NONE;
	file->unknown88 = NONE;
	file->unknown90 = NONE;
	file->unknown94 = 0;
	file->unknown98 = 0;
	file->unknown9c = 0;
	file->unknown7a = true;
	file->unknown8c = unknown8c;
	file->location = *location;
	return file;
}

// @retail 0x2ad380
bool file_copy_read_chunk(s_file_copy *copy)
{
	bool result = false;
	dword offset = copy->read_offset;
	dword size = copy->size - offset;
	dword bytes;

	if (size >= FILE_COPY_CHUNK_SIZE)
	{
		size = FILE_COPY_CHUNK_SIZE;
	}
	if (SetFilePointer(copy->source, offset, NULL, FILE_BEGIN) == copy->read_offset)
	{
		if (ReadFile(copy->source, copy->buffer, size, &bytes, NULL) && size == bytes)
		{
			copy->read_offset += bytes;
			result = true;
		}
	}
	return result;
}

// @retail 0x2ad400
bool file_copy_write_chunk(s_file_copy *copy)
{
	bool result = false;
	dword offset = copy->write_offset;
	dword size = copy->size - offset;
	dword bytes;

	if (size >= FILE_COPY_CHUNK_SIZE)
	{
		size = FILE_COPY_CHUNK_SIZE;
	}
	if (SetFilePointer(copy->destination, offset, NULL, FILE_BEGIN) == copy->write_offset)
	{
		if (WriteFile(copy->destination, copy->buffer, size, &bytes, NULL) && size == bytes)
		{
			copy->write_offset += bytes;
			result = true;
		}
	}
	return result;
}

// @retail 0x2ad5b0
void file_copy_close(s_file_copy *copy)
{
	if (copy->source != INVALID_HANDLE_VALUE)
	{
		CloseHandle(copy->source);
		copy->source = INVALID_HANDLE_VALUE;
	}
	if (copy->destination)
	{
		CloseHandle(copy->destination);
		copy->source = INVALID_HANDLE_VALUE;
	}
}

inline void saved_game_file_build_path(char *path, const char *folder, long type)
{
	strncpy(path, folder, 0x100);
	path[0xff] = 0;
	function_122810(path, function_216b60(type));
}

// @retail 0x2ad760
void saved_game_file_get_path(char *path, const s_saved_game_file *file)
{
	if (file->temporary)
	{
		strncpy(path, "t:\\blam", 0x100);
		path[0xff] = 0;
	}
	else
	{
		saved_game_file_build_path(path, file->location.name, file->location.type);
	}
}

bool g_5020d4;

enum
{
	_signed_file_read_header = 0,
	_signed_file_read_body,
	_signed_file_read_signature
};

/* a signed saved game file read in steps: a header, then a body, then the
   signature after both */
#pragma pack(push, 1)
struct s_signed_file_read_parameters
{
	void *header;
	dword header_size;
	byte *body;
	dword body_size;
	HANDLE file;
	dword body_offset;
	HANDLE signature_handle;
	s_saved_game_file_task *task;
	bool non_roamable;
	char state;
};
#pragma pack(pop)

// @retail 0x2ad7c0
PRIVATE long __stdcall signed_file_read_work(s_async_task *task, s_signed_file_read_parameters *parameters, long parameters_size)
{
	dword total = parameters->header_size + parameters->body_size + sizeof(XCALCSIG_SIGNATURE);
	real total_size = (real)total;
	dword bytes;

	parameters->task->state = 4;
	if (!parameters->task->unknown1)
	{
		switch (parameters->state)
		{
		case _signed_file_read_header:
			parameters->file = CreateFileA(parameters->task->path, GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
			if (parameters->file != INVALID_HANDLE_VALUE &&
				ReadFile(parameters->file, parameters->header, parameters->header_size, &bytes, NULL) &&
				bytes == parameters->header_size)
			{
				parameters->signature_handle = XCalculateSignatureBegin(parameters->non_roamable ? XCALCSIG_FLAG_NON_ROAMABLE : 0);
				if (parameters->signature_handle != INVALID_HANDLE_VALUE &&
					(!parameters->header_size || XCalculateSignatureUpdate(parameters->signature_handle, (const BYTE *)parameters->header, parameters->header_size) == ERROR_SUCCESS))
				{
					parameters->task->progress = (real)parameters->header_size / total_size;
					parameters->state = _signed_file_read_body;
					return 0;
				}
			}
			break;

		case _signed_file_read_body:
			if (parameters->body_offset < parameters->body_size)
			{
				byte *buffer;
				dword size;
				if (parameters->body)
				{
					buffer = parameters->body + parameters->body_offset;
					size = parameters->body_size - parameters->body_offset;
				}
				else
				{
					buffer = (byte *)g_5020c8.unknown04;
					size = g_5020c8.unknown08;
					g_5020c8.unknown00 = (dword)task;
					g_5020d4 = g_5020c8.unknown04 != 0x4fa0c8;
					if (size > parameters->body_size - parameters->body_offset)
					{
						size = parameters->body_size - parameters->body_offset;
					}
				}

				dword position = parameters->header_size + parameters->body_offset;
				if (SetFilePointer(parameters->file, position, NULL, FILE_BEGIN) == position &&
					ReadFile(parameters->file, buffer, size, &bytes, NULL) && bytes == size &&
					XCalculateSignatureUpdate(parameters->signature_handle, buffer, bytes) == ERROR_SUCCESS)
				{
					parameters->body_offset += bytes;
					if (parameters->body_offset >= parameters->body_size)
					{
						parameters->state = _signed_file_read_signature;
					}
					parameters->task->progress = (real)(parameters->header_size + parameters->body_offset) / total_size;
					return 0;
				}
				break;
			}
			parameters->state = _signed_file_read_signature;
			return 0;

		default:
		{
			dword position = parameters->body_size + parameters->header_size;
			XCALCSIG_SIGNATURE stored;
			XCALCSIG_SIGNATURE computed;

			if (SetFilePointer(parameters->file, position, NULL, FILE_BEGIN) == position &&
				ReadFile(parameters->file, &stored, sizeof(stored), &bytes, NULL) && bytes == sizeof(stored))
			{
				if (XCalculateSignatureEnd(parameters->signature_handle, &computed) == ERROR_SUCCESS)
				{
					parameters->task->succeeded = memcmp(&computed, &stored, sizeof(stored)) == 0;
				}
				parameters->signature_handle = INVALID_HANDLE_VALUE;
			}
			parameters->task->progress = 1.0f;
			break;
		}
		}
	}

	if (parameters->file != INVALID_HANDLE_VALUE)
	{
		CloseHandle(parameters->file);
	}
	if (parameters->signature_handle != INVALID_HANDLE_VALUE)
	{
		XCalculateSignatureEnd(parameters->signature_handle, NULL);
	}
	return 1;
}

// @retail 0x2ada70
bool signed_file_read_begin(void *header, dword header_size, void *body, dword body_size, bool non_roamable, s_saved_game_file_task *task, const char *path)
{
	s_signed_file_read_parameters parameters;

	task->unknown1 = false;
	task->succeeded = false;
	task->progress = 0.0f;
	task->state = 4;
	csstrnzcpy(task->path, path, sizeof(task->path));
	parameters.header = header;
	parameters.header_size = header_size;
	parameters.body = (byte *)body;
	parameters.body_size = body_size;
	parameters.file = INVALID_HANDLE_VALUE;
	parameters.body_offset = 0;
	parameters.signature_handle = INVALID_HANDLE_VALUE;
	parameters.task = task;
	parameters.non_roamable = non_roamable;
	parameters.state = _signed_file_read_header;
	return async_task_add_work((async_work_callback)signed_file_read_work, sizeof(parameters), &parameters, 2, &task->done) != NONE;
}

/* the job thread's scratch buffer, claimed by a task */
inline byte *job_thread_buffer_get(s_async_task *task, dword *size)
{
	byte *buffer = (byte *)g_5020c8.unknown04;
	g_5020d4 = buffer != (byte *)0x4fa0c8;
	*size = g_5020c8.unknown08;
	g_5020c8.unknown00 = (dword)task;
	return buffer;
}

enum
{
	_signed_file_write_header = 0,
	_signed_file_write_extend,
	_signed_file_write_body,
	_signed_file_write_signature
};

/* a signed saved game file write in steps; without a header it only makes
   sure the file has its full size */
#pragma pack(push, 1)
struct s_signed_file_write_parameters
{
	long state;
	void *header;
	dword header_size;
	void *body;
	dword body_size;
	HANDLE file;
	HANDLE signature_handle;
	s_saved_game_file_task *task;
	bool non_roamable;
	bool no_header;
};
#pragma pack(pop)

// @retail 0x2adb10
PRIVATE long __stdcall signed_file_write_work(s_async_task *task, s_signed_file_write_parameters *parameters, long parameters_size)
{
	long result = 1;

	parameters->task->state = 4;
	switch (parameters->state)
	{
	case _signed_file_write_header:
		parameters->file = CreateFileA(parameters->task->path, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);
		if (parameters->file != INVALID_HANDLE_VALUE)
		{
			if (parameters->no_header)
			{
				dword file_size = GetFileSize(parameters->file, NULL);
				if (file_size != INVALID_FILE_SIZE)
				{
					if (file_size < parameters->body_size + parameters->header_size + sizeof(XCALCSIG_SIGNATURE))
					{
						parameters->state = _signed_file_write_extend;
					}
					else
					{
						parameters->task->state = 0;
						parameters->task->succeeded = true;
						break;
					}
				}
			}
			else
			{
				parameters->signature_handle = XCalculateSignatureBegin(parameters->non_roamable ? XCALCSIG_FLAG_NON_ROAMABLE : 0);
				if (parameters->signature_handle != INVALID_HANDLE_VALUE &&
					XCalculateSignatureUpdate(parameters->signature_handle, (const BYTE *)parameters->header, parameters->header_size) == ERROR_SUCCESS)
				{
					parameters->state = _signed_file_write_extend;
				}
			}

			dword buffer_size;
			byte *buffer = job_thread_buffer_get(task, &buffer_size);
			memset(buffer, 0, MIN(buffer_size, parameters->header_size));

			bool success = true;
			for (dword offset = 0; offset < parameters->header_size && success; offset += buffer_size)
			{
				dword size = MIN(parameters->header_size - offset, buffer_size);
				dword bytes;
				success = SetFilePointer(parameters->file, offset, NULL, FILE_BEGIN) == offset &&
					WriteFile(parameters->file, buffer, size, &bytes, NULL) && bytes == size;
			}
			if (success)
			{
				result = 0;
			}
		}
		break;

	case _signed_file_write_extend:
		if (parameters->no_header)
		{
			dword buffer_size;
			byte *buffer = job_thread_buffer_get(task, &buffer_size);
			long offset = parameters->body_size - buffer_size + parameters->header_size;
			offset = offset > 0 ? offset : 0;
			memset(buffer, 0, buffer_size);
			dword size = MIN(parameters->body_size - offset + parameters->header_size, buffer_size);
			dword bytes;
			if (SetFilePointer(parameters->file, offset, NULL, FILE_BEGIN) == offset &&
				WriteFile(parameters->file, buffer, size, &bytes, NULL) && size == bytes)
			{
				parameters->state = _signed_file_write_body;
				result = 0;
			}
		}
		else
		{
			dword position = parameters->header_size;
			dword body_size = parameters->body_size;
			const BYTE *body = (const BYTE *)parameters->body;
			dword bytes;
			if (SetFilePointer(parameters->file, position, NULL, FILE_BEGIN) == position &&
				WriteFile(parameters->file, body, body_size, &bytes, NULL) && bytes == body_size &&
				XCalculateSignatureUpdate(parameters->signature_handle, body, bytes) == ERROR_SUCCESS)
			{
				parameters->state = _signed_file_write_body;
				result = 0;
			}
		}
		break;

	case _signed_file_write_body:
	{
		dword bytes;
		if (parameters->no_header ||
			SetFilePointer(parameters->file, 0, NULL, FILE_BEGIN) == 0 &&
			WriteFile(parameters->file, parameters->header, parameters->header_size, &bytes, NULL) && bytes == parameters->header_size)
		{
			parameters->state = _signed_file_write_signature;
			result = 0;
		}
		break;
	}

	case _signed_file_write_signature:
	{
		XCALCSIG_SIGNATURE signature;
		dword bytes;
		bool success = true;
		if (parameters->no_header)
		{
			memset(&signature, 0, sizeof(signature));
		}
		else
		{
			success = XCalculateSignatureEnd(parameters->signature_handle, &signature) == ERROR_SUCCESS;
			parameters->signature_handle = INVALID_HANDLE_VALUE;
		}
		dword position = parameters->header_size + parameters->body_size;
		if (success &&
			SetFilePointer(parameters->file, position, NULL, FILE_BEGIN) == position &&
			WriteFile(parameters->file, &signature, sizeof(signature), &bytes, NULL) && bytes == sizeof(signature))
		{
			parameters->task->state = 0;
			parameters->task->succeeded = true;
		}
		break;
	}

	default:
		__assume(0);
	}

	if (result)
	{
		if (parameters->file != INVALID_HANDLE_VALUE)
		{
			CloseHandle(parameters->file);
		}
		if (parameters->signature_handle != INVALID_HANDLE_VALUE)
		{
			XCalculateSignatureEnd(parameters->signature_handle, NULL);
		}
	}
	return result;
}

// @retail 0x2adec0
bool signed_file_write_begin(void *header, dword header_size, void *body, dword body_size, bool non_roamable, s_saved_game_file_task *task, const char *path)
{
	s_signed_file_write_parameters parameters;

	task->unknown1 = false;
	task->succeeded = false;
	task->progress = -1.0f;
	task->state = 4;
	csstrnzcpy(task->path, path, sizeof(task->path));
	parameters.state = _signed_file_write_header;
	parameters.header = header;
	parameters.header_size = header_size;
	parameters.body = body;
	parameters.body_size = body_size;
	parameters.file = INVALID_HANDLE_VALUE;
	parameters.signature_handle = INVALID_HANDLE_VALUE;
	parameters.task = task;
	parameters.non_roamable = non_roamable;
	parameters.no_header = header == NULL;
	return async_task_add_work((async_work_callback)signed_file_write_work, sizeof(parameters), &parameters, 2, &task->done) != NONE;
}
// @retail 0x2adf70
void saved_game_file_read(file_reference *file, void *buffer, dword size, bool non_roamable, s_saved_game_file_task *task)
{
	dword error;

	if (function_136970(file, 0x11, &error))
	{
		task->state = 3;
		if (file_read(file, size, true, buffer))
		{
			HANDLE signature_handle = XCalculateSignatureBegin(non_roamable ? XCALCSIG_FLAG_NON_ROAMABLE : 0);
			if (signature_handle != INVALID_HANDLE_VALUE)
			{
				XCALCSIG_SIGNATURE computed;
				XCALCSIG_SIGNATURE stored;

				XCalculateSignatureUpdate(signature_handle, (const BYTE *)buffer, size);
				if (XCalculateSignatureEnd(signature_handle, &computed) == ERROR_SUCCESS &&
					file_read_from_position(file, size, sizeof(stored), false, &stored) &&
					memcmp(&computed, &stored, sizeof(stored)) == 0)
				{
					task->succeeded = true;
					task->state = 0;
				}
			}
		}
		file_close(file);
	}
}

// @retail 0x2ae090
void saved_game_file_write(file_reference *file, void *buffer, dword size, bool non_roamable, s_saved_game_file_task *task)
{
	dword error;

	if (function_136970(file, 2, &error))
	{
		task->state = 2;
		if (file_write(file, size, buffer))
		{
			HANDLE signature_handle = XCalculateSignatureBegin(non_roamable ? XCALCSIG_FLAG_NON_ROAMABLE : 0);
			if (signature_handle != INVALID_HANDLE_VALUE)
			{
				XCALCSIG_SIGNATURE signature;

				XCalculateSignatureUpdate(signature_handle, (const BYTE *)buffer, size);
				if (XCalculateSignatureEnd(signature_handle, &signature) == ERROR_SUCCESS &&
					file_write_to_position(file, size, sizeof(signature), &signature))
				{
					task->succeeded = true;
					task->state = 0;
				}
			}
		}
		file_close(file);
	}
}
