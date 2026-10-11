
#define TEST_FRAME_TIME 500
#include "maindep/engine.h"
// битовае маски (N - 1), тоже что и x % N, где N - степень двойки
#define MICRO_STEP 24	// Если (frame_count % ONLINE_BITMASK) == 0, пробуждаем поток данных в кадре(звук, активная логика)
#define MACRO_STEP 255	// Если (frame_count % SOUNDS_BITMASK) == 0, пробуждаем мировой поток
#ifdef TEST_FRAME_TIME
#define TEST_FRAME_FUNC(frame_func) frame_func
#else
#define TEST_FRAME_FUNC(frame_func) ;
#endif
#ifdef TEST_POSITION
#define TEST_POSITION_FUNC(position_func) position_func
#else
#define TEST_POSITION_FUNC(position_func) ;
#endif

i_4b main() {
	TEST_FRAME_FUNC(
			u_8b count = 0;
			f_8b accum = 0;
		);
	u_1b frame_count = 0;
	Bool run_flag = true;
	Logger.Init();

	emg::Data.Window.Params.Size.width_ = 1280;	//Width
	emg::Data.Window.Params.Size.heigth_ = 720;	//Height
	emg::Data.Window.Params.Flags.glfw_smpls_ = 0;
	emg::Data.Window.Params.Flags.vert_sinkh_ = 1;
	emg::Data.Window.Params.Flags.wind_focus_ = 1;
	emg::Data.Window.Params.Flags.wind_showd_ = 1;
	emg::Data.Window.Params.Flags.cursorshow_ = 1;
	emg::Data.Window.Params.Flags.showbutton_ = 0;
	emg::Data.Window.Params.Flags.fullscreen_ = 0;
	emg::Data.Window.Params.Flags.mxsolution_ = 0;

	emg::Data.Window.Params.Input.key_nums_[EMG_INPUT_EXIT]	= 27;						//ESC
	emg::Data.Window.Params.Input.key_nums_[EMG_INPUT_FRONT]= static_cast<i_2b>('W');	//Вперед
	emg::Data.Window.Params.Input.key_nums_[EMG_INPUT_BACK]	= static_cast<i_2b>('S');	//Назад
	emg::Data.Window.Params.Input.key_nums_[EMG_INPUT_LEFT]	= static_cast<i_2b>('A');	//Влево
	emg::Data.Window.Params.Input.key_nums_[EMG_INPUT_RIGTH]= static_cast<i_2b>('D');	//Вправо
	emg::Data.Window.Params.Input.key_nums_[EMG_INPUT_UP]	= static_cast<i_2b>('Z');	//Вверх
	emg::Data.Window.Params.Input.key_nums_[EMG_INPUT_DOWN]	= static_cast<i_2b>('X');	//Вниз
	emg::Data.Window.Params.Input.mouse_sensitivity_ = 0.0025f;

	emg::Data.Files.files_data_map_= "data/data_map.mem";

	emg::Data.Render.Camera.fov_ = 75.0f;
	emg::Data.Render.Camera.far_ = 4096.0f;//16*96
	emg::Data.Render.Camera.near_ = 0.1f;
	emg::Data.Render.Camera.velocity_ = 0.08f;

	if (Logger.error_flag_ >= 0) emg::window::Init();
	if (Logger.error_flag_ >= 0) emg::render::Init();
	if (Logger.error_flag_ >= 0) emg::logics::Init();

	if (Logger.error_flag_ >= 0) {
		thrd::GM_Thread offline_world_thread(emg::logics::WorldMamagerTic);
		thrd::GM_Thread online_world_thread(emg::logics::LocalManagerTic);
		while (run_flag && emg::window::IsRunned()) {
			//if (emg::window::GetTime() < 0.0157f) continue;
			++frame_count;
			emg::window::UpdateEvent();
			if (emg::Data.Window.Input.key_flags_[EMG_INPUT_EXIT]) {
				run_flag = false;
				online_world_thread.stop();
				offline_world_thread.stop();
			}
			else if ((frame_count % MICRO_STEP) == 0) 
				online_world_thread.step();
			else if ((frame_count % MACRO_STEP) == 0) 
				offline_world_thread.step();
			emg::render::CreateFrame();
			emg::window::UpdateFrame();

			TEST_FRAME_FUNC(
					accum += emg::window::GetTime();
					emg::window::ReSetTime();
					if (++count >= TEST_FRAME_TIME) {
						run_flag = false;
						online_world_thread.stop();
						offline_world_thread.stop();
					}
				); 
			TEST_POSITION_FUNC(
					std::cout
					<< "X:  " << emg::Data.Render.Camera.postn_.x
					<< "Y:  " << emg::Data.Render.Camera.postn_.y
					<< "Z:  " << emg::Data.Render.Camera.postn_.z
					<< std::endl
				);
		}//while(run && !emg::Data.Window.IsRunned())
	}//if(Logger.error_flag_ >= 0)
	emg::logics::Exit();
	emg::render::Exit();
	emg::window::Exit();
	Logger.Exit();
	TEST_FRAME_FUNC(
			std::cout
			<< "\nRENDER FARAME TEST RESULT : "
			<< "\n"
			<< "\n\tTEST FRAME COUNTER [ " << static_cast<f_8b>(count) << " ];"
			<< "\n\tTEST FRAME TIMER   [ " << static_cast<f_8b>(accum) << " ];"
			<< "\n"
			<< "\n\tMIDDLE FRAME TIME  [ " << static_cast<f_8b>(accum / static_cast<f_8b>(count)) << " ];"
			<< "\n\tMIDDLE FPS         [ " << static_cast<f_8b>(1.0f) / (accum / static_cast<f_8b>(count)) << " ];"
			<< std::endl;
			system("pause");
		);
	TEST_POSITION_FUNC(_CrtDumpMemoryLeaks());
	return 0;
}
