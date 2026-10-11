#pragma once
#ifndef MY_LIB_ERROR_STRUCT
#define MY_LIB_ERROR_STRUCT

struct {
	i_1b error_flag_ = -1;
	__forceinline void Init() {
		error_flag_ = 0;
#ifdef _DEBUG
		std::time_t currentTime;
		std::tm localTime;
		char time_buffer[80];
		// Получение текущего времени
		currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		// Безопасное преобразование в локальное время (Windows)
		localtime_s(&localTime, &currentTime);
		asctime_s(time_buffer, sizeof(time_buffer), &localTime);
		//Вывод наных в файл лога
		std::ofstream log_file("ERROR_LOG_FILE.txt", std::ios::app);
		log_file
			<< "\n================================================================================"
			<< "\n============[   D E B U G   L O G G E R   I N I T I A L I Z E D   ]=============" 
			<< "\n TIME :  " << static_cast<t_wrd>(time_buffer) << std::endl;
		log_file.close();
#ifdef EMG_LOGER_STDOUT
		std::cout
			<< "\n================================================================================"
			<< "\n============[   D E B U G   L O G G E R   I N I T I A L I Z E D   ]============="
			<< "\n TIME :  " << static_cast<t_wrd>(time_buffer) << std::endl;
#endif //EMG_LOGER_STDOUT
#endif //_DEBUG
	}
	__forceinline void SetError(c_wrd err_descr, i_1b _error_code = -1) {
		std::ofstream log_file("ERROR_LOG_FILE.txt", std::ios::app);
		log_file << err_descr;
		log_file.close();
		error_flag_ = _error_code;
#ifdef EMG_LOGER_STDOUT
		std::cout << err_descr;
#endif //EMG_LOGER_STDOUT
	}
	__forceinline void SetError(i_8b err_descr, i_1b _error_code = -1) {
		std::ofstream log_file("ERROR_LOG_FILE.txt", std::ios::app);
		log_file << err_descr;
		log_file.close();
		error_flag_ = _error_code;
#ifdef EMG_LOGER_STDOUT
		std::cout << err_descr;
#endif //EMG_LOGER_STDOUT
	}
	__forceinline void Exit() {
		if (error_flag_ < 0) {
			std::ofstream log_file("ERROR_LOG_FILE.txt", std::ios::app);
			std::time_t currentTime;
			std::tm localTime;
			char time_buffer[80];
			// Получение текущего времени
			currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			// Безопасное преобразование в локальное время (Windows)
			localtime_s(&localTime, &currentTime);
			asctime_s(time_buffer, sizeof(time_buffer), &localTime);
			//Вывод наных в файл лога
			log_file << "EXIT_PROGRAM_TIME :  " << static_cast<t_wrd>(time_buffer);
			log_file.close();
		}
#ifdef _DEBUG
		std::ofstream log_file("ERROR_LOG_FILE.txt", std::ios::app);
		log_file 
			<< "\n=========[   D E B U G   L O G G E R   W O R K   C O M P L E T E D   ]=========="
			<< "\n================================================================================" 
			<< "\n\n\n\n\n\n\n\n"<<std::endl;
		log_file.close();
#ifdef EMG_LOGER_STDOUT
		if (error_flag_ < 0) 
			std::cout
				<< "\n================================================================================"
				<< "\n=========================|----------------------------|========================="
				<< "\n=========================| ERROR LOG IS WRITED BLIYAT |========================="
				<< "\n=========================|----------------------------|========================="
				<< "\n================================================================================"
				<< std::endl;
		std::cout
			<< "\n=========[   D E B U G   L O G G E R   W O R K   C O M P L E T E D   ]=========="
			<< "\n================================================================================"
			<< "\n\n\n\n\n\n\n\n" << std::endl;
#endif //EMG_LOGER_STDOUT
#endif //_DEBUG
	}
} Logger;

#endif // MY_LIB_ERROR_STRUCT