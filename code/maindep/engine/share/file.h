#pragma once

#ifndef MY_LIB_FILE_INTERFACE
#define MY_LIB_FILE_INTERFACE

namespace iof {
	FILE*		file_ptr_;	// Указатель на файл
	// ОДИН ПОТОК. Полная загрузка файла. Обязательно: free(data) !
	__forceinline
	char* load_file_txt(c_wrd file_name, u_8b* File_size) {
		fopen_s(&file_ptr_, file_name, "rb");
		if (!file_ptr_) {
			Logger.SetError("ERROR::FILE::OPEN:: ERROR OPEN FILE [ ");
			Logger.SetError(file_name);
			Logger.SetError(" ]\n");
			return nullptr;
		}
		fseek(file_ptr_, 0, SEEK_END); // Перемещаем указатель в конец файла
		*File_size = ftell(file_ptr_);  // Получаем размер
		rewind(file_ptr_); // Возвращаем указатель в начало
		char* loadable_text = (char*)malloc(*File_size + 1);
		if (!loadable_text) { 
			Logger.SetError("ERROR::FILE::READ:: ERROR READ DATA [ ");
			Logger.SetError(file_name);
			Logger.SetError(" ]\n");
			return nullptr; 
		}
		fread(loadable_text, *File_size, 1, file_ptr_);
		fclose(file_ptr_);
		loadable_text[*File_size] = '\0';
		return loadable_text;// std::move(loadable_text);
	}
	// ОДИН ПОТОК. Полная загрузка файла. Обязательно: free(data) !
	__forceinline
	PMem load_file_bin(c_wrd file_name, u_8b* File_size) {
		fopen_s(&file_ptr_, file_name, "rb");
		if (!file_ptr_) {
			Logger.SetError("ERROR::FILE::OPEN:: ERROR OPEN FILE [ ");
			Logger.SetError(file_name);
			Logger.SetError(" ]\n");
			return nullptr;
		}
		fseek(file_ptr_, 0, SEEK_END); // Перемещаем указатель в конец файла
		*File_size = ftell(file_ptr_);  // Получаем размер
		rewind(file_ptr_); // Возвращаем указатель в начало
		PMem loadable_data = malloc(*File_size);
		if (!loadable_data) {
			Logger.SetError("ERROR::FILE::READ:: ERROR READ DATA [ ");
			Logger.SetError(file_name);
			Logger.SetError(" ]\n");
			return nullptr;
		}
		fread(loadable_data, *File_size, 1, file_ptr_);
		fclose(file_ptr_);
		return loadable_data;// std::move(loadable_data);
	}

	// ОДИН ПОТОК. Просто получить размер файла в байтах
	__forceinline
	u_4b file_size(c_wrd file_name, u_8b& File_size) {
		fopen_s(&file_ptr_, file_name, "rb"); 
		if(!file_ptr_) { return 0; }
		fseek(file_ptr_, 0, SEEK_END); // Перемещаем указатель в конец файла
		File_size = ftell(file_ptr_);  // Получаем размер
		rewind(file_ptr_); // Возвращаем указатель в начало
		fclose(file_ptr_);
	}
	// ОДИН ПОТОК. Загрузить файл без проверок
	__forceinline
	void load_bin(c_wrd file_name, PMem* loadable_data, const u_8b& data_size) {
		fopen_s(&file_ptr_, file_name, "rb");
		if(!file_ptr_) { loadable_data = nullptr; return; }
		fread(*loadable_data, data_size, 1, file_ptr_);
		fclose(file_ptr_);
	}
	// ОДИН ПОТОК. Сохранить файл без проверок
	__forceinline
	void save_bin(c_wrd file_name, PMem* saveable_data, const u_8b& data_size) {
		fopen_s(&file_ptr_, file_name, "wb");
		if(!file_ptr_) { saveable_data = nullptr; return; }
		fwrite(*saveable_data, data_size, 1, file_ptr_);
		fclose(file_ptr_);
	}


//---------------------------------------------------------------------------------------------------------
//----------------------------------------SUPER-FAST-FUNCTION-TEST-----------------------------------------
//---------------------------------------------------------------------------------------------------------

	// ОДИН ПОТОК. Free data after load_file_ultrafast(file_name, bytes_size)
	__forceinline void free_ultrafast_loaded_data(PMem data) {
#ifdef _WIN32
		UnmapViewOfFile(data);
#else //LINUX
		free(data);
#endif
	}

	// ОДИН ПОТОК. Free data func: free_ultrafast_loaded_data(*data)!!!
	__forceinline PMem load_file_ultrafast(c_wrd file_name, u_8b* bytes_size) {
#ifdef _WIN32
		// 1. Windows API (в 5-10 раз быстрее стандартных функций)
		HANDLE hFile = CreateFileA(
			file_name,
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_NO_BUFFERING,
			nullptr
		);
		if (hFile == INVALID_HANDLE_VALUE) {
			Logger.SetError("ERROR::FILE::ULTRAFAST:: INVALID HANDLE VALUE [ ");
			Logger.SetError(file_name);
			Logger.SetError(" ]\n");
			return nullptr;
		}
		LARGE_INTEGER fileSize;
		GetFileSizeEx(hFile, &fileSize);
		*bytes_size = static_cast<u_8b>(fileSize.QuadPart);
		// 2. PMem-mapped файлы для мгновенного доступа
		HANDLE hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
		if (!hMapping) {
			Logger.SetError("ERROR::FILE::ULTRAFAST:: INVALID MAPPING [ ");
			Logger.SetError(file_name);
			Logger.SetError(" ]\n");
			CloseHandle(hFile);
			return nullptr;
		}
		PMem data = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, *bytes_size);
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return data;

#else //LINUX
		// 3. Linux/UNIX оптимизация (mmap + O_DIRECT)
		int fd = open(file_name, O_RDONLY | O_DIRECT);
		if (fd == -1) return nullptr;
		struct stat st;
		fstat(fd, &st);
		*bytes_size = st.st_size;
		// 4. Выровненная память для O_DIRECT (512b/4K alignment)
		PMem buf = nullptr;
		posix_memalign(&buf, 4096, *bytes_size);
		// 5. Асинхронное чтение через pread
		ssize_t read = pread(fd, buf, *bytes_size, 0);
		close(fd);
		if (read != static_cast<ssize_t>(*bytes_size)) {
			free(buf);
			return nullptr;
		}
		return buf;
#endif
	}
};

#endif // MY_LIB_FILE_INTERFACE