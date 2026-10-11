#pragma once
#ifndef MY_LIB_WINDOW_REALISATION
#define MY_LIB_WINDOW_REALISATION
#define emg_DET emg::Data.Window.DetailData
#define emg_PAR emg::Data.Window.Params
#define emg_INP emg::Data.Window.Input

// Сбросить таймер окна
__forceinline void emg::window::ReSetTime() { glfwSetTime(0); }
// Возвращает значение таймера окна
__forceinline f_8b emg::window::GetTime() { return glfwGetTime(); }
// Выводит данные из ФреймБуффера
__forceinline void emg::window::UpdateFrame() { glfwSwapBuffers(emg_DET.win_refc_); }
// Возвращает true - если окно запущено, false - Закрыто
__forceinline Bool emg::window::IsRunned() { return glfwWindowShouldClose(emg_DET.win_refc_) == 0; }
// Обловляет ввод и события окна (обязательно)
__forceinline void emg::window::UpdateEvent() {
	glfwPollEvents();
#ifdef _WIN32
	if (!GetKeyboardState(emg_DET.key_bytes_)) {
		Logger.SetError("ERROR::WINDOW::KEYBROAD READ FAIL");
		return;
	}
	for (i_4b i = 0; i < KEYS_LIMIT; ++i) {
		emg_INP.key_flags_[i] = emg_DET.key_bytes_[emg_PAR.Input.key_nums_[i]] & 0x80;
	}
#else #else //LINUX
	for (i_4b i = 0; i < KEYS_LIMIT; ++i) {
		emg_PAR.Input.key_nums_[i] = glfwGetKey(emg_WND.win_refc_, emg_PAR.Input.key_nums_[i]) ? 0x80 : 0;
	}
#endif
	glfwGetCursorPos(emg_DET.win_refc_, &(emg_INP.cursor_displacement_.x), &(emg_INP.cursor_displacement_.y));
	glfwSetCursorPos(emg_DET.win_refc_, emg_DET.half_width_, emg_DET.half_heigth_);
	emg_INP.cursor_displacement_.x = emg_DET.half_width_ - emg_INP.cursor_displacement_.x;
	emg_INP.cursor_displacement_.y = emg_DET.half_heigth_ - emg_INP.cursor_displacement_.y;
}
// Oчистка ресурсов. Завершение работы GLFW
__forceinline void emg::window::Exit() {
	glfwDestroyWindow(emg_DET.win_refc_);
	glfwTerminate();
}
// Инициализация окна
__forceinline void emg::window::Init() {
	// 0. Загрузка параметров окна и ввода
	emg_DET.mon_refc_ = NULL;	//Указатель на монитор, для полного окна
	// 1. Инициализация и Настройка GLFW
	if (!glfwInit()) {
		Logger.SetError("Failed to initialize GLFW\n");
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);					// Версия OpenGL 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);					// Версия OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// Профиль Core
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);			// Удаляем устаревшие функции
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);							// Окно видимо
	glfwWindowHint(GLFW_DECORATED, emg_PAR.Flags.showbutton_);	// Рамка и кнопки окна
	glfwWindowHint(GLFW_RESIZABLE, emg_PAR.Flags.wind_showd_);	// Отключаем возможность ресайза окна
	glfwWindowHint(GLFW_FOCUSED, emg_PAR.Flags.wind_focus_);	// Фокус на окне
	glfwWindowHint(GLFW_SAMPLES, emg_PAR.Flags.glfw_smpls_);	// MSAA для сглаживания
	glfwWindowHint(GLFW_STENCIL_BITS, 0);				// Экономим на станциле
	glfwWindowHint(GLFW_DEPTH_BITS, 24);	//Размерность буффера глубины
	glfwWindowHint(GLFW_RED_BITS,	8);		//Глубина цвета растреризации КРАСНЫЙ
	glfwWindowHint(GLFW_GREEN_BITS, 8);		//Глубина цвета растреризации ЗЕЛЕНЫЙ
	glfwWindowHint(GLFW_BLUE_BITS,	8);		//Глубина цвета растреризации СИНИЙ
	glfwWindowHint(GLFW_ALPHA_BITS, 8);		//Глубина цвета растреризации АЛЬФА

	// 2. Настройка параметров окна
	if (emg_PAR.Flags.fullscreen_) {
		emg_DET.mon_refc_ = glfwGetPrimaryMonitor();
	}
	if (emg_PAR.Flags.mxsolution_) {
		emg_DET.mon_refc_ = glfwGetPrimaryMonitor();
		emg_PAR.Size.width_ = (glfwGetVideoMode(emg_DET.mon_refc_)->width);
		emg_PAR.Size.heigth_ = (glfwGetVideoMode(emg_DET.mon_refc_)->height);
	}

	// 3. Запись середины окна и коэфицента соотношения сторон
	emg_DET.half_width_ = (f_4b)emg_PAR.Size.width_ / 2.0f; //half_Width
	emg_DET.half_heigth_ = (f_4b)emg_PAR.Size.heigth_ / 2.0f; //half_Height
	emg_DET.scale_koef_ = (f_4b)emg_PAR.Size.width_ / (f_4b)emg_PAR.Size.heigth_;

	// 4. Создание окна
	emg_DET.win_refc_ = glfwCreateWindow(emg_PAR.Size.width_, emg_PAR.Size.heigth_, "Window", emg_DET.mon_refc_, NULL);
	if (!emg_DET.win_refc_) Logger.SetError("Create window ERROR\n");

	// 5. Сделать контекст окна текущим
	glfwSetCursorPos(emg_DET.win_refc_, emg_DET.half_width_, emg_DET.half_heigth_);
	glfwMakeContextCurrent(emg_DET.win_refc_);
	glfwGetFramebufferSize(emg_DET.win_refc_, &emg_PAR.Size.width_, &emg_PAR.Size.heigth_);
	if (emg_PAR.Flags.vert_sinkh_) {
		glfwSwapInterval(1); // Вертикальная синхронизация
	}
}
#undef emg_DET
#undef emg_PAR
#undef emg_INP
#endif //MY_LIB_WINDOW_REALISATION