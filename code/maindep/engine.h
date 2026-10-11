#pragma once
#ifndef SIMPLE_ENGINE_FOR_GAME_LIBS
#define SIMPLE_ENGINE_FOR_GAME_LIBS
//-------------------------------------------------------------------------
#ifdef _DEBUG
	//#define ONLINE_STDOUT
	//#define OFFLINE_STDOUT
	//#define EMG_ALLOC_STDOUT
	//#define EMG_LOGIC_STDOUT
	//#define EMG_LOGER_STDOUT
	//#define EMG_CUL_FACE_OFF
	//#define LOAD_CMD_STDOUT
	//#define DRAW_CMD_STDOUT
	//#define DRAW_ATR_STDOUT
	//#define DRAW_ENT_STDOUT
	//#define MTH_LIST_STDOUT
	#define DBGONLY(only_debug_process) only_debug_process;
#else
	#define DBGONLY(only_debug_process) ;
#endif // _DEBUG
#ifdef NDEBUG
#ifndef TEST_FRAME_TIME
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#endif // TEST_FRAME_TIME
#pragma comment(linker, "/ENTRY:mainCRTStartup")
#endif // NDEBUG
//-------------------------------------------------------------------------
#pragma comment( lib, "opengl32.lib" )  //Добавить в линковкщик либу openGL
#pragma comment( lib, "glfw3.lib" )     //Добавить в линковкщик либу GLfw
#pragma warning(disable: 4201)    // Отключить warning об анонимных структурах

#define MTH_ARR_QUEUE 16			// Используем структуру с массивом signed short int c интерфейсом очереди
#define MTH_LIST_STRCT				// Использовать закрытый односвязный список
#define VC_EXTRALEAN				// Дополнительное сокращение#ifdef _WIN32
#define STBI_ONLY_PNG				// Включить только формат PNG
#define STB_IMAGE_IMPLEMENTATION	// Подключение библиотеки STB_IMAGE
#define GLM_FORCE_SIMD_AVX2			// Использовать AVX2 инструкции
#define GLM_FORCE_INTRINSICS		// Включить SSE/AVX интринсики
#define GLM_FORCE_DEFAULT_ALIGNED	// Автовыравнивание для SIMD
#define GLFW_INCLUDE_NONE			// Убираем сторонне подкулючение GLFW
#define WIN32_LEAN_AND_MEAN			// Убирает редко используемые части Windows API
#define DUMBPATHER_DEFAULT_FAST		// Поиск пути по умолчанию считает ячейку легкопроходимой
#define SPoint2u mth::U2_2pos

#ifdef EMG_ALLOC_STDOUT
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

//Подключаем глобальные библиотеки
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <GLM/glm.hpp>
#include <GLAD/glad.h>
#include <GLAD/glad.c>
#include <GLFW/glfw3.h>

#include <STB/stb_image.h>

#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>

//Подключаем локальные библиотеки
#include "engine/share/math.h"	//namespace mth
#include "engine/share/alrm.h"	//struct errlog
#include "engine/share/imag.h"	//namespace mth
#include "engine/share/file.h"	//namespace iof
#include "engine/share/thrd.h"	//namespace thrd
#include "engine/share/path.h"	//namespace thrd

#define SLP_THRD(a) std::this_thread::sleep_for(std::chrono::milliseconds(a)) // Поток засыпает на (а) миллисекунд

#define KEYS_LIMIT 256
#define CHUNK_SIZE 16
#define WORLD_SIZE   64	 // 10 bit <- use xy 2 byte WORLD_POS
#define REGION_SIZE  64	 //  6 bit <- use xy 2 byte WORLD_POS
#define TERRAIN_SIZE 128 // DRAW_POS
#define TEXTUR_LIMIT 32//Тектсутры
#define MODELS_LIMIT 32//Модели
#define ONLINE_LIMIT 32//Экземпляры
#define ENTITY_LIMIT 128//32768//Сущности
#define BEHEVIOR_PARAMS 8

//ИНДЕКСЫ КОДОВ КЛАВИШ
#define EMG_INPUT_EXIT 0
#define EMG_INPUT_FRONT 1
#define EMG_INPUT_BACK 2
#define EMG_INPUT_RIGTH 4
#define EMG_INPUT_LEFT 3
#define EMG_INPUT_UP 5
#define EMG_INPUT_DOWN 6

namespace emg {
	struct {//  LIMIT: 12 544 / 1 073 741 824 bytes
		public:
			struct { //render;
				mth::SInstnAttrib InstnAttribs_[ONLINE_LIMIT]{};	// Массив атрибутов
				mth::SDrawCommand DrawCommands_[MODELS_LIMIT]{};	// Массив комманд
				struct {
					u_4b textures_index_[TEXTUR_LIMIT];	// ID текстурного буфера
					u_4b index_VAO_{};				// Индекс вершинного массива
					u_4b index_VBO_{};				// Индекс вершинного буффера
					u_4b index_UBO_{};				// Индекс матрицы пространства(камеры) в шейдере
					u_4b index_comands_inderect_{};	// Индекс массива комманд
					u_4b index_attribs_inderect_{};	// Индекс массива аттрибутов в шейдере
					u_4b index_matrixs_inderect_{};	// Индекс массива модельных матриц в шейдере
					u_4b index_compute_shader_{};	// Индекс шейдера расчетов
					u_4b index_drawind_shader_{};	// Индекс шейдера отрисовки
				} GPU;
				struct {//Камера
					m4_64b projection_matrix_{};// Матрица проекции
					m4_64b cam_view_matrix_{};	// Матрица вида(камеры)
					v3_12b null_view_{};
					v3_12b ang_front_{};
					v3_12b angle_{};
					v3_12b postn_{};
					f_4b velocity_{};
					f_4b far_{};	//Дистанция отрисовки
					f_4b near_{};	//Плоскость отсечения
					f_4b fov_{};	//Угол обора
				} Camera;
				PMem  DrawCommands_refc_{};	//Быстрая ссылка на матрицу Пространства
				PMem  DrawAttribut_refc_{};	//Быстрая ссылка на матрицу Пространства
				f_4b* ViewMatrix_refc_{};	//Быстрая ссылка на матрицу Пространства
				i_4b  ViewMatrix_size_{};	//Размер матрицы Пространства
				i_4b count_texutres_{};		// Количество текстур и атласов
				i_4b count_instaces_{};		// Количество инстансов в кадре
				i_4b count_commands_{};		// Количество команд отрисовки
				i_4b count_work_group_{};	//Количество рабочих групп в шейдере
				u_2b essence_add_to_show_id_{};
				u_2b essence_add_to_hide_id_{};
				std::atomic<Bool> draw_command_update_{};	// Массив команд отрисовки изменен
				std::atomic<Bool> draw_command_is_free_{};	// Массив команд отрисовки доступен
			} Render;
			struct { //window;
				struct {//ПОЛЕ ПАРАМЕТРОВ:
					struct {
						i_2b key_nums_[KEYS_LIMIT]{};
						f_4b mouse_sensitivity_{};
					} Input;
					struct {//Параметры окна
						i_1b glfw_smpls_{}; //Сглаживание
						i_1b vert_sinkh_{}; //Вертикальная синхронизация
						i_1b wind_focus_{}; //Фокус на окне
						i_1b wind_showd_{}; //Видимость окна
						i_1b cursorshow_{}; //Курсор
						i_1b showbutton_{}; //Кнопки окна
						i_1b fullscreen_{}; //Полноэкран
						i_1b mxsolution_{}; //Макс.Разрешение
					} Flags;
					struct {//Размеры окна
						i_4b width_{};//Ширина
						i_4b heigth_{};//Высота
					} Size;
				//---------------------------------------
				} Params;
				struct {
					mth::F8_2pos cursor_displacement_{};
					u_1b key_flags_[KEYS_LIMIT]{};
				} Input;
				struct {//Полуразмеры окна
					u_1b key_bytes_[KEYS_LIMIT]{};
					GLFWmonitor* mon_refc_{};	//Указатель на монитор
					GLFWwindow* win_refc_{};	//Указатель на окно
					f_8b half_heigth_{};		//f_4b H;
					f_8b half_width_{};			//f_4b W;
					f_4b scale_koef_{};			//Соотношение сторон окна
				} DetailData;
				//i_2b : 1; //Макс.Разрешение
			} Window;
			struct {
				mth::U2_2pos	  unit_global_step_[ENTITY_LIMIT]{};	// Массив данных позиции
				mth::SEssenceMeta unit_visual_data_[ENTITY_LIMIT]{};	// Массив данных модели
				mth::SI2b_ArrQueue queue_add_to_draw_{};
				mth::SI2b_ManagerList online_manager_list_{};
				mth::SI2b_ForwardList online_list_{};
			} Logic;
			struct {
				c_wrd files_data_map_{};
			} Files;
	} Data;
	namespace window { // ПРОЦЕДУРЫ ОКНA
		__forceinline extern void Init();
		__forceinline extern void Exit();
		__forceinline extern void UpdateFrame();
		__forceinline extern void UpdateEvent();
		__forceinline extern void ReSetTime();
		__forceinline extern Bool IsRunned();
		__forceinline extern f_8b GetTime();
	}
	namespace render { // ПРОЦЕДУРЫ РЕНДЕРА
		__forceinline extern void Init();
		__forceinline extern void Exit();
		__forceinline extern void CreateFrame();
		__forceinline extern void InserInRenderPipline();
		__forceinline extern void EraseInRenderPipline();
	}
	namespace logics {
		__forceinline extern void Init();
		__forceinline extern void Exit();
		__forceinline extern void WorldMamagerTic();
		__forceinline extern void LocalManagerTic();
	}


	namespace iofile {
		enum FILES_TYPE : u_1b {
			COMP_SHADER,
			VERT_SHADER,
			FRAG_SHADER,
			ATLS_TEXTUR,
			MESH_STRUCT
		};
		struct ShaderLoadedData {
			t_wrd text{ nullptr };
		};
		struct TexturLoadedData {
			t_wrd text{ nullptr };
		};
		struct MeshLoadetData {
			mth::SDrawCommand* DrawCmd{ nullptr };
			f_4b* Vrtx{ nullptr };
		};
		__forceinline extern void Init();
		__forceinline extern PMem Load(emg::iofile::FILES_TYPE fType);
	}
	namespace sounds {
	}
}

#include "engine/systems/window.h"
#include "engine/systems/render.h"
#include "engine/systems/sounds.h"
#include "engine/systems/logics.h"
#include "engine/systems/iofile.h"

#endif //SIMPLE_ENGINE_FOR_GAME_LIBS