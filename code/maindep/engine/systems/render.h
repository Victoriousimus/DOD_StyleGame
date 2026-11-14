#pragma once
#ifndef MY_LIB_RENDER_REALISATION
#define MY_LIB_RENDER_REALISATION
//===========================================================================================
#define CHECK_3_FIRST(word, a, b, c) (word)[i] == (a) && (word)[i+1] == (b) && (word)[i+2] == (c)
#define CHECK_3_LAST(word) (word)[i+3] == ':' && (word)[i+4] == '=' && (word)[i+5] == ' '
#define CHECK_6_SYMBOLS_cmp(word) CHECK_3_FIRST( (word), 'c', 'm', 'p') && CHECK_3_LAST( (word) )
#define CHECK_6_SYMBOLS_frg(word) CHECK_3_FIRST( (word), 'f', 'r', 'g') && CHECK_3_LAST( (word) )
#define CHECK_6_SYMBOLS_vrt(word) CHECK_3_FIRST( (word), 'v', 'r', 't') && CHECK_3_LAST( (word) )
#define CHECK_6_SYMBOLS_rvt(word) CHECK_3_FIRST( (word), 'r', 'v', 't') && CHECK_3_LAST( (word) )
#define CHECK_6_SYMBOLS_rfg(word) CHECK_3_FIRST( (word), 'r', 'f', 'g') && CHECK_3_LAST( (word) )
#define CHECK_6_SYMBOLS_msh(word) CHECK_3_FIRST( (word), 'm', 's', 'h') && CHECK_3_LAST( (word) )
#define CHECK_6_SYMBOLS_atl(word) CHECK_3_FIRST( (word), 'a', 't', 'l') && CHECK_3_LAST( (word) )
#define CHECK_6_SYMBOLS_map(word) CHECK_3_FIRST( (word), 'm', 'a', 'p') && CHECK_3_LAST( (word) )

#define emg_Region emg::Data.Render.DrawRegion
#define emg_Buffer emg::Data.Render.GPU
#define emg_Window emg::Data.Window
#define emg_Render emg::Data.Render
#define TEXTURES_LOAD_MODE 3

#ifdef LOAD_CMD_STDOUT
#define COUT_CMD(cout_cmd_procedure) cout_cmd_procedure
#else
#define COUT_CMD(cout_cmd_procedure)
#endif

struct SLoadingShaders {
	u_4b models_VertShader;
	u_4b models_FragShader;
	u_4b region_VertShader;
	u_4b region_FragShader;
	u_4b compute_Shader;
	i_4b success_loading;
};

// загрузка региона
__forceinline void Interpolation_UpdateLendspaces() {
	//glNamedBufferSubData(emg_Region.index_meshes_VBO_, slot * REGION_VERTEX_SIZE, bytes, data);
}
// Обновление массива атрибутов
__forceinline void Interpolation_UpdateAttributes() {
	for (i_4b i = 0; i < emg_Render.count_instaces_; ++i) {
		//emg_ATR[i].yaw += 0.01f;
	}
}

// Считать ввод для камеры, изменить позицию и углы
__forceinline void Interpolation_UpdateCameraData() {
	emg_Render.Camera.angle_.x += static_cast<f_4b>(emg_Window.Input.cursor_displacement_.x) * emg_Window.Params.Input.mouse_sensitivity_;
	emg_Render.Camera.angle_.y += static_cast<f_4b>(emg_Window.Input.cursor_displacement_.y) * emg_Window.Params.Input.mouse_sensitivity_;
	//Нормализуем поворот по осям
	if (emg_Render.Camera.angle_.x < 0) emg_Render.Camera.angle_.x += MTH_RADIANS;
	else if (emg_Render.Camera.angle_.x > MTH_RADIANS) emg_Render.Camera.angle_.x -= MTH_RADIANS;
	if (emg_Render.Camera.angle_.y > MTH_MAX_ANG_Y) emg_Render.Camera.angle_.y = MTH_MAX_ANG_Y;
	else if (emg_Render.Camera.angle_.y < MTH_MIN_ANG_Y) emg_Render.Camera.angle_.y = MTH_MIN_ANG_Y;
	//Запицем для оптимизации вызова cos(x), sin(x)
	emg_Render.Camera.ang_front_.x = sinf(emg_Render.Camera.angle_.x);
	emg_Render.Camera.ang_front_.z = cosf(emg_Render.Camera.angle_.x);
	//Считываем смещение по осям
	if (emg_Window.Input.key_flags_[EMG_INPUT_FRONT]) {
		emg_Render.Camera.postn_.x += emg_Render.Camera.velocity_ * emg_Render.Camera.ang_front_.x;
		emg_Render.Camera.postn_.z += emg_Render.Camera.velocity_ * emg_Render.Camera.ang_front_.z;
	}
	if (emg_Window.Input.key_flags_[EMG_INPUT_BACK]) {
		emg_Render.Camera.postn_.x -= emg_Render.Camera.velocity_ * emg_Render.Camera.ang_front_.x;
		emg_Render.Camera.postn_.z -= emg_Render.Camera.velocity_ * emg_Render.Camera.ang_front_.z;
	}
	if (emg_Window.Input.key_flags_[EMG_INPUT_LEFT]) {
		emg_Render.Camera.postn_.x += emg_Render.Camera.velocity_ * emg_Render.Camera.ang_front_.z;
		emg_Render.Camera.postn_.z -= emg_Render.Camera.velocity_ * emg_Render.Camera.ang_front_.x;
	}
	if (emg_Window.Input.key_flags_[EMG_INPUT_RIGTH]) {
		emg_Render.Camera.postn_.x -= emg_Render.Camera.velocity_ * emg_Render.Camera.ang_front_.z;
		emg_Render.Camera.postn_.z += emg_Render.Camera.velocity_ * emg_Render.Camera.ang_front_.x;
	}
	// Вверх / Вниз
	if (emg_Window.Input.key_flags_[EMG_INPUT_UP])   emg_Render.Camera.postn_.y += emg_Render.Camera.velocity_; //Z
	if (emg_Window.Input.key_flags_[EMG_INPUT_DOWN]) emg_Render.Camera.postn_.y -= emg_Render.Camera.velocity_; //X
	//Обновим матрицу вида
	emg_Render.Camera.ang_front_.y = cosf(emg_Render.Camera.angle_.y);
	emg_Render.Camera.ang_front_.x *= emg_Render.Camera.ang_front_.y;
	emg_Render.Camera.ang_front_.z *= emg_Render.Camera.ang_front_.y;
	emg_Render.Camera.ang_front_.y = sinf(emg_Render.Camera.angle_.y);
	emg_Render.Camera.ang_front_ = glm::normalize(emg_Render.Camera.ang_front_);
	emg_Render.Camera.cam_view_matrix_ = emg_Render.Camera.projection_matrix_ *
		glm::lookAt( emg_Render.Camera.postn_, emg_Render.Camera.postn_ + emg_Render.Camera.ang_front_, emg_Render.Camera.null_view_ );
}

//Загрузка шейдерных программ
__forceinline void load_saheder_progs(SLoadingShaders* SLoadingShaders) {
	u_4b& shdr_Cmp = SLoadingShaders->compute_Shader;
	u_4b& shdr_m_Vrt = SLoadingShaders->models_VertShader;
	u_4b& shdr_m_Frg = SLoadingShaders->models_FragShader;
	u_4b& shdr_r_Vrt = SLoadingShaders->region_VertShader;
	u_4b& shdr_r_Frg = SLoadingShaders->region_FragShader;
	i_4b& success = SLoadingShaders->success_loading;
	u_8b lengt_accum = 0;
	t_wrd sahderName{ nullptr };
	t_wrd fileText{ nullptr };
	//Загружаем файл карты данных
	fileText = iof::load_file_txt(emg::Data.Files.files_data_map_, &lengt_accum);
	if (!fileText) {
		Logger.SetError("ERROR::[ render.h ]: load_prog_to_GPU(): Shader_List load [");
		Logger.SetError(emg::Data.Files.files_data_map_);
		Logger.SetError(" ] \n");
		return;
	}
	//Проходим по всем символам
	for (u_4b j, i = 0; i < lengt_accum - 5; ++i) {
		if (fileText[i + 4] != '=') continue;
		u_4b* shader_ref{ nullptr };
		u_4b shader_type = 0;
		//Проверяем тэги в поисках интересующего символа
		if (CHECK_6_SYMBOLS_cmp(fileText)) {
			shader_type = GL_COMPUTE_SHADER;
			shader_ref = &shdr_Cmp;
		}
		else
		if (CHECK_6_SYMBOLS_frg(fileText)) {
			shader_type = GL_FRAGMENT_SHADER;
			shader_ref = &shdr_m_Frg;
		}
		else
		if (CHECK_6_SYMBOLS_vrt(fileText)) {
			shader_type = GL_VERTEX_SHADER;
			shader_ref = &shdr_m_Vrt;
		}
		else
		if (CHECK_6_SYMBOLS_rfg(fileText)) {
			shader_type = GL_FRAGMENT_SHADER;
			shader_ref = &shdr_r_Frg;
		}
		else
		if (CHECK_6_SYMBOLS_rvt(fileText)) {
			shader_type = GL_VERTEX_SHADER;
			shader_ref = &shdr_r_Vrt;
		}
		else continue;
		//если символ шейдера найден, записываем его название
		sahderName = static_cast<t_wrd>(malloc(64));
		if (!sahderName) {
			if (shader_ref == &shdr_Cmp)
				Logger.SetError("ERROR::[ render.h ]: load_prog_to_GPU(): cmp:= : sahderName = malloc(64)\n");
			if (shader_ref == &shdr_m_Frg)
				Logger.SetError("ERROR::[ render.h ]: load_prog_to_GPU(): frg:= : sahderName = malloc(64)\n");
			if (shader_ref == &shdr_m_Vrt)
				Logger.SetError("ERROR::[ render.h ]: load_prog_to_GPU(): vrt:= : sahderName = malloc(64)\n");
			if (shader_ref == &shdr_r_Frg)
				Logger.SetError("ERROR::[ render.h ]: load_prog_to_GPU(): rfg:= : sahderName = malloc(64)\n");
			if (shader_ref == &shdr_r_Vrt)
				Logger.SetError("ERROR::[ render.h ]: load_prog_to_GPU(): rvt:= : sahderName = malloc(64)\n");
			return;
		}
		for (j = 0, i += 6; i < lengt_accum; ++i, ++j) {
			sahderName[j] = fileText[i];
			if (sahderName[j] == '*') break;
		} sahderName[j] = '\0';
		// Загрузка шейдерa из текстового файла
		u_8b Shader_file_size;
		t_wrd shaderText = iof::load_file_txt(sahderName, &Shader_file_size);
		// Создание и компиляция шейдера
		*shader_ref = glCreateShader(shader_type);
		glShaderSource(*shader_ref, 1, &shaderText, NULL);
		glCompileShader(*shader_ref);
		// Проверка ошибок компиляции шейдера
		glGetShaderiv(*shader_ref, GL_COMPILE_STATUS, &success);
		if (!success) {
			t_sym infoLog[512];
			glGetShaderInfoLog(*shader_ref, 512, NULL, infoLog);
			Logger.SetError("ERROR::SHADER::COMPILATION_FAILED [ ");
			Logger.SetError(sahderName);
			Logger.SetError(" ]\n");
			Logger.SetError(infoLog);
			if (shdr_Cmp)   glDeleteShader(shdr_Cmp);
			if (shdr_m_Vrt) glDeleteShader(shdr_m_Vrt);
			if (shdr_m_Frg) glDeleteShader(shdr_m_Frg);
			if (shdr_r_Vrt) glDeleteShader(shdr_r_Vrt);
			if (shdr_r_Frg) glDeleteShader(shdr_r_Frg);
			free(sahderName);
			free(shaderText);
			free(fileText);
			return;
		}
		// Освобождение памяти
		free(shaderText);
		free(sahderName);
	}
	free(fileText);
}
// Загрузка массивов аттрибутов вершин моделей
__forceinline void load_all_vetex_data(f_4b** result_array, u_8b* attrib_count, i_4b* moDels_count) {
	f_4b* all_VertexAttribsArray;
	f_4b** vecArr = nullptr;	// указатель на массивы вершин
	u_8b* vecLen = nullptr;		// указатели на длинны массивов
	u_8b lengt_accum = 0;
	i_4b models_count = 0;
	t_wrd fileNames[MODELS_LIMIT] = { 0 };
	t_wrd fileText = { 0 };
	fileText = iof::load_file_txt(emg::Data.Files.files_data_map_, &lengt_accum);
	if (!fileText) {
		Logger.SetError("ERROR::[ render.h ]: MoDels_List load [");
		Logger.SetError(emg::Data.Files.files_data_map_);
		Logger.SetError(" ] \n");
		*result_array = nullptr;
		*moDels_count = 0;
		*attrib_count = 0;
		return;
	}
	for (i_4b j, i = 0; i < lengt_accum - 5; ++i) {
		if(CHECK_6_SYMBOLS_msh(fileText)) {
			fileNames[models_count] = new t_sym[64];
			if (!fileNames[models_count]) {
				Logger.SetError("ERROR::[ render.h ]: load_all_vetex_data(result_array, attrib_count): fileNames[");
				Logger.SetError(models_count);
				Logger.SetError("] MALLOC ERROR\n");
				for (j = 0; j < models_count; ++j) { delete[] fileNames[j]; }
				free(fileText);
				*result_array = nullptr;
				*moDels_count = 0;
				*attrib_count = 0;
				return;
			}
			for (j = 0, i += 6; i < lengt_accum; ++i, ++j) {
				fileNames[models_count][j] = fileText[i];
				if (fileNames[models_count][j] == '*') break;
			}
			fileNames[models_count][j] = '\0';
			++models_count;
		}
	} 
	free(fileText);
	// Инициализируем временное хранилище памяти для штучных мешей, для дальнейшего объеденения
	vecArr = new f_4b*[models_count];	// <- Массив указателей на масссивы вершин
	vecLen = new u_8b[models_count];	// <- Массив длин массивов по указателю
	if (!vecArr || !vecLen) {
		Logger.SetError("ERROR::[ render.h ]:vecArr = malloc(models_count * size); vecLen = malloc(models_count * size)\n");
		for (i_4b i = 0; i < models_count; ++i) { delete[] fileNames[i]; }
		if(vecLen) delete[] vecLen;
		if(vecArr) delete[] vecArr;
		*result_array = nullptr;
		*moDels_count = 0;
		*attrib_count = 0;
		return;
	}
	lengt_accum = 0;
	for (i_4b i = 0; i < models_count; ++i) { vecArr[i] = nullptr; }
	// Загружаем во вершины в массивы и формируем под него DrawComand
	COUT_CMD( std::cout << "--------------------------------------------------\n" << std::endl );
	for (i_4b i = 0; i < models_count; ++i) {
		vecArr[i] = static_cast<f_4b*>(iof::load_file_ultrafast(fileNames[i], &vecLen[i]));// vecLen[i] - будет записана длинна массива в байтах
		delete[] fileNames[i];
		if (!vecArr[i]) {
			Logger.SetError("ERROR::[ render.h ]:vecArr[i] = static_cast<f_4b*>(iof::load_file_ultrafast(fileNames[i], &vertx_count));\n");
			// Подчищаем временные хранилища мешей
			for (i_4b j = i; j > -1; --j) { iof::free_ultrafast_loaded_data(vecArr[j]); }
			for (i_4b j = 0; j < models_count; ++j) { delete[] fileNames[j]; }
			delete[] vecLen;
			delete[] vecArr;
			*result_array = nullptr; 
			*moDels_count = 0;
			*attrib_count = 0;
			return;
		}
		mth::SDrawCommand& ItrCmd  = emg_Render.DrawCommands_[i];
		mth::SDrawCommand& PrevCmd = emg_Render.DrawCommands_[i-1];
		ItrCmd.inst_count = i < 1 ? 1 : 0; // Количество инстансов
		ItrCmd.vert_count = static_cast<u_4b>(vecLen[i]) >> 5;	// Количество вершин для примитива
		ItrCmd.inst_first = i < 1 ? 0 : PrevCmd.inst_first + PrevCmd.inst_count; // Базовый инстанс (добавляется к gl_instance_index)
		ItrCmd.vert_first = i < 1 ? 0 : PrevCmd.vert_first + PrevCmd.vert_count; // Начальный индекс в вершинном массиве
		vecLen[i] = static_cast<u_4b>(vecLen[i] >> 2);			// Сдвиг чтобы сичтать f_4b
		lengt_accum += vecLen[i];
		COUT_CMD(
				std::cout
				<< "------------------------------------\n"
				<< "[ inst_first : " << ItrCmd.inst_first << " ]; "
				<< "[ inst_count : " << ItrCmd.inst_count << " ]; "
				<< "[ vert_first : " << ItrCmd.vert_first << " ]; "
				<< "[ vert_count : " << ItrCmd.vert_count << " ]; "
				<< "------------------------------------\n"
				<< std::endl;
			);
	}
	COUT_CMD( std::cout<< "--------------------------------------------------\n" << std::endl );
	all_VertexAttribsArray = static_cast<f_4b*>(malloc(lengt_accum * sizeof(f_4b)));
	if (!all_VertexAttribsArray) {
		Logger.SetError("ERROR::[ render.h ]: static_cast<f_4b*>(malloc(lengt_accum * sizeof(f_4b)))\n");
		for (i_4b i = 0; i < models_count;++i) { iof::free_ultrafast_loaded_data(vecArr[i]); }
		delete[] vecLen;
		delete[] vecArr;
		*result_array = nullptr;
		*moDels_count = 0;
		*attrib_count = 0;
		return;
	}
	for (i_4b OnVec = 0, p = 0, i = 0; i < lengt_accum; ++i, ++p) {
		if (p == vecLen[OnVec]) {
			iof::free_ultrafast_loaded_data(vecArr[OnVec]);
			p = 0; ++OnVec;
			if (OnVec >= models_count) {
				Logger.SetError("ERROR::[ render.h ]: UUUU SYKA! OUT OF ARRAY\n");
				break;
			}
		}
		all_VertexAttribsArray[i] = vecArr[OnVec][p];
	}
	delete[] vecLen;
	delete[] vecArr;
	*moDels_count = models_count;
	*attrib_count = lengt_accum;
	*result_array = all_VertexAttribsArray;
}

// Init() < - Настройка параметров отрисовки
__forceinline void init_draw_params() {
	if (!gladLoadGL()) {
		Logger.SetError("ERROR::[ render.h ]:Failed to initialize GLAD\n");
		return;
	}
	glViewport(0, 0, emg_Window.Params.Size.width_, emg_Window.Params.Size.heigth_);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	#ifndef EMG_CUL_FACE_OFF
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);	// Направление обхода вершин (против часовой)
	glCullFace(GL_BACK);	// Отсекать задние грани
	#endif // EMG_CUL_FACE_OFF
	glDisable(GL_STENCIL_TEST); // Если не используется
	glDisable(GL_BLEND); // Если нет прозрачных объектов
}
// Init() < - Mассив трансформаций инстансов: attrib_ptr_draw[]. Заполняет матрицы: mProj, mView, mSpace
__forceinline void init_view_matrix() {
	emg_Render.draw_command_update_.store(false); // Массив команд отрисовки изменен
	emg_Render.draw_command_is_free_.store(true); // Массив команд отрисовки изменен
	emg_Render.count_work_group_ = 0;
	emg_Render.count_instaces_ = 0;
	emg_Render.count_commands_ = 0;
	emg_Render.Camera.angle_.x = 5.94f;
	emg_Render.Camera.angle_.y = -0.7f;
	emg_Render.Camera.postn_.x = 11.66f;
	emg_Render.Camera.postn_.y = 18.29f;
	emg_Render.Camera.postn_.z = -26.1f;
	emg_Render.Camera.projection_matrix_ = glm::perspective(
		glm::radians(emg_Render.Camera.fov_),
		emg::Data.Window.DetailData.scale_koef_,
		emg_Render.Camera.near_,
		emg_Render.Camera.far_
	);
	emg_Render.Camera.null_view_ = v3_12b(0.0f, 1.0f, 0.0f);
	emg_Render.Camera.cam_view_matrix_ = m4_64b(1) * emg_Render.Camera.projection_matrix_;
	emg_Render.ViewMatrix_size_ = sizeof(m4_64b);
	emg_Render.ViewMatrix_refc_ = glm::value_ptr(emg_Render.Camera.cam_view_matrix_);
	for (u_4b i = 0; i < ONLINE_LIMIT; ++i) {
		mth::SInstnAttrib& ItrAttr = emg_Render.InstnAttribs_[i];
		ItrAttr.pich = 0; //Тангаж
		ItrAttr.yaw  = 0; //Рысканье
		ItrAttr.roll = 0; //Крен
		ItrAttr.x = 0; //положительные = влево
		ItrAttr.y = 0; //положительные = дальш
		ItrAttr.z = 0; //положительные = выше
	}
}

// Init() < - Загрузка шейдера расчета и отрисовки в GPU
__forceinline void load_prog_to_GPU() {
	SLoadingShaders Prog{ 0 };
	load_saheder_progs(&Prog);
	if (!Prog.models_FragShader) Logger.SetError("ERROR::SHADER::PROGRAM::LOST [MODELS FRAGMENT SHADER]\n");
	if (!Prog.models_VertShader) Logger.SetError("ERROR::SHADER::PROGRAM::LOST [MODELS VERTEX SHADER]\n");
	if (!Prog.region_FragShader) Logger.SetError("ERROR::SHADER::PROGRAM::LOST [REGION FRAGMENT SHADER]\n");
	if (!Prog.region_VertShader) Logger.SetError("ERROR::SHADER::PROGRAM::LOST [REGION VERTEX SHADER]\n");
	if (!Prog.compute_Shader) Logger.SetError("ERROR::SHADER::PROGRAM::LOST [COMPUTE SHADER]\n");
	if (Logger.error_flag_ < 0) return;

	// Создание шейдерной программы и линковка расчетного шейдера
	emg_Buffer.index_compute_shader_ = glCreateProgram();
	glAttachShader(emg_Buffer.index_compute_shader_, Prog.compute_Shader);
	glLinkProgram(emg_Buffer.index_compute_shader_);
	glGetProgramiv(emg_Buffer.index_compute_shader_, GL_LINK_STATUS, &Prog.success_loading);
	if (!Prog.success_loading) {
		glDeleteShader(Prog.compute_Shader);
		glDeleteShader(Prog.models_VertShader);
		glDeleteShader(Prog.models_FragShader);
		char infoLog[512];
		glGetProgramInfoLog(emg_Buffer.index_compute_shader_, 512, NULL, infoLog);
		Logger.SetError("ERROR::SHADER::PROGRAM::LINKING_FAILED [COMPUTE SHADER]  LOG:\n");
		Logger.SetError(infoLog);
		Logger.SetError("\n");
		return;
	}
	// Создание шейдерной программы и линковка программы для объектов
	emg_Buffer.index_draw_models_shader_ = glCreateProgram();
	glAttachShader(emg_Buffer.index_draw_models_shader_, Prog.models_VertShader);
	glAttachShader(emg_Buffer.index_draw_models_shader_, Prog.models_FragShader);
	glLinkProgram(emg_Buffer.index_draw_models_shader_);
	glGetProgramiv(emg_Buffer.index_draw_models_shader_, GL_LINK_STATUS, &Prog.success_loading);
	if (!Prog.success_loading) {
		glDeleteShader(Prog.compute_Shader);
		glDeleteShader(Prog.models_VertShader);
		glDeleteShader(Prog.models_FragShader);
		char infoLog[1024];
		glGetProgramInfoLog(emg_Buffer.index_draw_models_shader_, 1024, NULL, infoLog);
		Logger.SetError("ERROR::SHADER::PROGRAM::LINKING_FAILED [ BASE_DRAW_SHADER ]  LOG:\n");
		Logger.SetError(infoLog);
		Logger.SetError("\n");
		return;
	}
	// Создание шейдерной программы и линковка программы для местности
	emg_Buffer.index_draw_region_shader_ = glCreateProgram();
	glAttachShader(emg_Buffer.index_draw_region_shader_, Prog.region_VertShader);
	glAttachShader(emg_Buffer.index_draw_region_shader_, Prog.region_FragShader);
	glLinkProgram(emg_Buffer.index_draw_region_shader_);
	glGetProgramiv(emg_Buffer.index_draw_region_shader_, GL_LINK_STATUS, &Prog.success_loading);
	if (!Prog.success_loading) {
		glDeleteShader(Prog.compute_Shader);
		glDeleteShader(Prog.models_VertShader);
		glDeleteShader(Prog.models_FragShader);
		glDeleteShader(Prog.region_VertShader);
		glDeleteShader(Prog.region_FragShader);
		char infoLog[1024];
		glGetProgramInfoLog(emg_Buffer.index_draw_region_shader_, 1024, NULL, infoLog);
		Logger.SetError("ERROR::SHADER::PROGRAM::LINKING_FAILED [ BASE_DRAW_SHADER ]  LOG:\n");
		Logger.SetError(infoLog);
		Logger.SetError("\n");
		return;
	}
	// Удаление шейдеров (они уже в программе)
	glDeleteShader(Prog.compute_Shader);
	glDeleteShader(Prog.models_VertShader);
	glDeleteShader(Prog.models_FragShader);
	glDeleteShader(Prog.region_VertShader);
	glDeleteShader(Prog.region_FragShader);
}
// Init() < - Загрузка атрибутов отрисовки в GPU
__forceinline void load_attr_to_GPU() {
	m4_64b InstancesMatrix[ONLINE_LIMIT] = {0};
	i_8b commands_byte_size = static_cast<i_8b>(MODELS_LIMIT) * 16;
	i_8b attribts_size = static_cast<i_8b>(ONLINE_LIMIT) * 32;
	//+++++++++++++++++[ emg_Render.Camera Buffer ]++++++++++++++++++
	glGenBuffers(1, &emg_Buffer.index_camera_UBO_);													// Создание буфера
	glBindBuffer(GL_UNIFORM_BUFFER, emg_Buffer.index_camera_UBO_);									// Захват буфера
	glBufferData(GL_UNIFORM_BUFFER, emg_Render.ViewMatrix_size_, nullptr, GL_DYNAMIC_DRAW);			// Настройка буфера
	glBindBufferBase(GL_UNIFORM_BUFFER, emg_Buffer.Layout::VwProj, emg_Buffer.index_camera_UBO_);			// Помечаем как ViewProj
	glBufferSubData(GL_UNIFORM_BUFFER, NULL, emg_Render.ViewMatrix_size_, emg_Render.ViewMatrix_refc_);// Загрузка буфера
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);															// Отпускаем буфер
	//+++++++++++++++++[ Matrix Buffer ]++++++++++++++++++
	glGenBuffers(1, &emg_Buffer.index_matrixs_inderect_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, emg_Buffer.index_matrixs_inderect_);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(InstancesMatrix), InstancesMatrix, NULL);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, emg_Buffer.Layout::Matrix, emg_Buffer.index_matrixs_inderect_);// Помечаем как матрицы
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
	//+++++++++++++++++[ Attrib Buffer ]++++++++++++++++++
	glGenBuffers(1, &emg_Buffer.index_attribs_inderect_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, emg_Buffer.index_attribs_inderect_);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, attribts_size, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	emg_Buffer.DrawModelsAttribut_refc_ =
		glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, attribts_size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, emg_Buffer.Layout::Attrib, emg_Buffer.index_attribs_inderect_);// Помечаем как аттрибуты
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
	//+++++++++++++++++[ Command Buffer ]+++++++++++++++++
	glGenBuffers(1, &emg_Buffer.index_comands_inderect_);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, emg_Buffer.index_comands_inderect_);
	glBufferStorage(GL_DRAW_INDIRECT_BUFFER, commands_byte_size, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	emg_Buffer.DrawModelsCommands_refc_ =
		glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, 0, commands_byte_size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, NULL);
	if (!emg_Buffer.DrawModelsAttribut_refc_) Logger.SetError("ERROR::MODELLS::UNMAPPED [emg_Buffer.DrawModelsAttribut_refc_]\n");
	if (!emg_Buffer.DrawModelsCommands_refc_) Logger.SetError("ERROR::MODELLS::UNMAPPED [emg_Buffer.DrawModelsCommands_refc_]\n");
}
// Init() < - Заполняет загружает и передает модели в матрицу [Важен порядок загрузки в "data_map.mem"]
__forceinline void load_mesh_to_GPU() {
	u_8b Vert_Сount;
	f_4b* All_ModelsVertexes;
	// vertx_count - будет записанo количество элемнтов массива (все вертексы в одном массиве)
	load_all_vetex_data(&All_ModelsVertexes, &Vert_Сount, &emg_Render.count_commands_);
	if (!Vert_Сount) return;
	//+++++++++++++++++[ Vertex Buffer ]++++++++++++++++++
	glGenVertexArrays(1, &emg_Buffer.index_meshes_VAO_);
	glBindVertexArray(emg_Buffer.index_meshes_VAO_);
	glGenBuffers(1, &emg_Buffer.index_meshes_VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, emg_Buffer.index_meshes_VBO_);
	glBufferStorage(GL_ARRAY_BUFFER, Vert_Сount * sizeof(f_4b), All_ModelsVertexes, NULL);
	glVertexAttribPointer(emg_Buffer.Layout::Pos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f_4b), (PMem)0);
	glEnableVertexAttribArray(emg_Buffer.Layout::Pos); // Координаты
	glVertexAttribPointer(emg_Buffer.Layout::Nrm, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f_4b), (PMem)(3 * sizeof(f_4b)));
	glEnableVertexAttribArray(emg_Buffer.Layout::Nrm); // Нормали
	glVertexAttribPointer(emg_Buffer.Layout::Tex, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f_4b), (PMem)(6 * sizeof(f_4b)));
	glEnableVertexAttribArray(emg_Buffer.Layout::Tex); // Текстура
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
	// Отчистка памяти массива мешей
	free(All_ModelsVertexes);
}
// Init() < - Подготавливает буфер для мешей террейна
__forceinline void init_wrld_in_GPU() {
	i_8b position_byte_size = static_cast<i_8b>(REGION_DRAWS_COUNT) * 8;//sizeof(f_4b)*2
	i_8b commands_byte_size = static_cast<i_8b>(REGION_BUFFER_COUNT) * 16;//sizeof(u_4b)*4
	i_8b vertexes_byte_size = static_cast<i_8b>(REGION_VERTEX_SIZE * REGION_BUFFER_COUNT) * 32 * 4;//(32)byte_vertex_size * (4)map_size_koef
	//+++++++++++++++++[ Command Buffer ]+++++++++++++++++
	glGenBuffers(1, &emg_Buffer.index_regions_command_);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, emg_Buffer.index_regions_command_);
	glBufferStorage(GL_DRAW_INDIRECT_BUFFER, commands_byte_size, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	emg_Buffer.DrawRegionCommands_refc_ =
		glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, 0, commands_byte_size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, NULL);
	//+++++++++++++++++[ Offsets Buffer ]+++++++++++++++++
	glGenBuffers(1, &emg_Buffer.index_regions_offsets_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, emg_Buffer.index_regions_offsets_);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, position_byte_size, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	emg_Buffer.DrawRegionOffsets_refc_ =
		glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, position_byte_size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, emg_Buffer.Layout::Offset, emg_Buffer.index_regions_offsets_); // Помечаем как RegionOffsets
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
	//+++++++++++++++++[ Vertex Buffer ]++++++++++++++++++
	glGenVertexArrays(1, &emg_Buffer.index_regions_VAO_);
	glBindVertexArray(emg_Buffer.index_regions_VAO_);
	glGenBuffers(1, &emg_Buffer.index_regions_VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, emg_Buffer.index_regions_VBO_);
	glBufferStorage(GL_ARRAY_BUFFER, vertexes_byte_size, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	emg_Buffer.DrawRegionVertexs_refc_ =
		glMapBufferRange(GL_ARRAY_BUFFER, 0, vertexes_byte_size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glVertexAttribPointer(emg_Buffer.Layout::Pos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f_4b), (PMem)0);
	glEnableVertexAttribArray(emg_Buffer.Layout::Pos); // Координаты
	glVertexAttribPointer(emg_Buffer.Layout::Nrm, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f_4b), (PMem)(3 * sizeof(f_4b)));
	glEnableVertexAttribArray(emg_Buffer.Layout::Nrm); // Нормали
	glVertexAttribPointer(emg_Buffer.Layout::Tex, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f_4b), (PMem)(6 * sizeof(f_4b)));
	glEnableVertexAttribArray(emg_Buffer.Layout::Tex); // Текстура
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
	if(!emg_Buffer.DrawRegionCommands_refc_) Logger.SetError("ERROR::LENDSPACE::UNMAPPED [emg_Buffer.DrawRegionCommands_refc_]\n");
	if(!emg_Buffer.DrawRegionOffsets_refc_)  Logger.SetError("ERROR::LENDSPACE::UNMAPPED [emg_Buffer.DrawRegionOffsets_refc_]\n");
	if(!emg_Buffer.DrawRegionVertexs_refc_)  Logger.SetError("ERROR::LENDSPACE::UNMAPPED [emg_Buffer.DrawRegionVertexs_refc_]\n");
	if (Logger.error_flag_ < 0) return;

	u_8b len = 0; 
	f_4b* vecArr = static_cast<f_4b*>(iof::load_file_ultrafast("data/models/meshes/world/m_000_000.vtn", &len));
	if (!vecArr || len < 1) {
		Logger.SetError("ERROR::LENDSPACE::LOAD FAIL [\"data/models/meshes/world/m_000_000.vtn\"]\n"); 
		return;
	}
	if (len > REGION_VERTEX_SIZE*32*4) {
		Logger.SetError("ERROR::LENDSPACE::FAIL MODEL SIZE("); 
		Logger.SetError(len);
		Logger.SetError(") [\"data/models/meshes/world/m_000_000.vtn\"]\n");
		return;
	}
	for (i_4b i = 0; i < REGION_BUFFER_COUNT; ++i) {
		PMem target_address = static_cast<f_4b*>(emg_Buffer.DrawRegionVertexs_refc_) + (i * REGION_VERTEX_SIZE * 8 * 4);
		if (!target_address) {
			Logger.SetError("ERROR::LENDSPACE::FAILADDR[emg_Buffer.DrawRegionVertexs_refc_ + (i * REGION_VERTEX_SIZE * 8 * 4)]\n");
			return;
		}
		memcpy(target_address, vecArr, len);
	}
	for (i_4b i = 0; i < REGION_BUFFER_COUNT; ++i) {
		mth::SDrawCommand& ItrCmd = emg_Render.RegionCommands_[i];
		mth::F4_2pos& ItrOffset = emg_Render.RegionOffsets_[i];
		ItrCmd.inst_count = 0;
		ItrCmd.inst_first = i < 1? 0 : emg_Render.RegionCommands_[i-1].inst_first +1;
		ItrCmd.vert_first = i * REGION_VERTEX_SIZE * 4;
		ItrCmd.vert_count = len / 32;
		if (i < REGION_DRAWS_COUNT) {
			f_4b pX = static_cast<f_4b>(-1 + (i / 3));
			f_4b pY = static_cast<f_4b>(-1 + (i % 3));
			ItrOffset.x = pX * 128.0f;
			ItrOffset.y = pY * 128.0f;
			ItrCmd.inst_count = 1;
		}
	}
	memcpy(emg_Buffer.DrawRegionOffsets_refc_,  emg_Render.RegionOffsets_,  position_byte_size);
	memcpy(emg_Buffer.DrawRegionCommands_refc_, emg_Render.RegionCommands_, commands_byte_size);
	iof::free_ultrafast_loaded_data(vecArr);

	u_8b error_code = glGetError();
	if (error_code != GL_NO_ERROR) {
		Logger.SetError("ERROR::LENDSPACE::LOAD FAIL [data/models/meshes/world/m_000_000.vtn]\n");
		Logger.SetError("GL_ERROR :"); Logger.SetError(error_code); Logger.SetError("\n");
	}
}

// Init() < - Загрузка буфферов в GPU
__forceinline void load_txtr_to_GPU() {
	i_4b x, y, n;
	t_wrd fileNames[TEXTUR_LIMIT];
	u_8b lengt_accum = 0;
	u_4b texture_count = 0;
	u_1b* data;
	t_wrd fileText;
	for (i_4b i = 0; i < TEXTUR_LIMIT; ++i) {
		fileNames[i] = nullptr;
	}
	fileText = iof::load_file_txt(emg::Data.Files.files_data_map_, &lengt_accum);
	if (!fileText) {
		Logger.SetError("ERROR::[ render.h ]: Textur_List load [");
		Logger.SetError(emg::Data.Files.files_data_map_);
		Logger.SetError(" ] \n");
		free(fileText);
		return;
	}
	for (u_4b j, i = 0; i < lengt_accum - 5; ++i) {
		if (CHECK_6_SYMBOLS_atl(fileText)) {
			fileNames[texture_count] = new t_sym[64];
			if (!fileNames[texture_count]) {
				Logger.SetError("ERROR::[ render.h ]: load_txtr_to_GPU(): atl:= : fileName = malloc(64)\n");
				while(texture_count > 0) {
					delete[] fileNames[--texture_count];
				}
				free(fileText);
				return;
			}
			for (j = 0, i += 6; i < lengt_accum; ++i, ++j) {
				fileNames[texture_count][j] = fileText[i];
				if (fileNames[texture_count][j] == '*') break;
			} fileNames[texture_count][j] = '\0';
			++texture_count;
		}
	}
	free(fileText);
	emg_Render.count_texutres_ = texture_count;
	glGenTextures(emg_Render.count_texutres_, emg_Buffer.textures_index_);
	for (u_4b i = 0; i < texture_count; ++i) {
		data = stbi_load(fileNames[i], &x, &y, &n, TEXTURES_LOAD_MODE);
		if (!data) {
			Logger.SetError("ERROR::[ render.h ]: texture load [");
			Logger.SetError(fileNames[i]);
			Logger.SetError(" ] \n"); 
			while ( i < texture_count) {
				delete[] fileNames[i]; ++i;
			}
			return;
		}
		glBindTexture(GL_TEXTURE_2D, emg_Buffer.textures_index_[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, NULL);
		stbi_image_free(data);
		delete[] fileNames[i];
	}
}

// Метод: Инифиализация рендера сцены
__forceinline void emg::render::Init() {
	init_draw_params(); if (Logger.error_flag_ < 0) return;
	init_view_matrix(); if (Logger.error_flag_ < 0) return;
	load_prog_to_GPU(); if (Logger.error_flag_ < 0) return;
	load_mesh_to_GPU(); if (Logger.error_flag_ < 0) return;
	load_attr_to_GPU(); if (Logger.error_flag_ < 0) return;
	load_txtr_to_GPU(); if (Logger.error_flag_ < 0) return;
	//init_wrld_in_GPU(); if (Logger.error_flag_ < 0) return;
}
// Метод: Oчистка ресурсов
__forceinline void emg::render::Exit() {
	// Дождаться завершения всех операций GPU
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glUseProgram(NULL);
	glFinish();
	// Отчистка ресурсов    
	if (emg_Buffer.DrawModelsAttribut_refc_) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, emg_Buffer.index_attribs_inderect_);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		emg_Buffer.DrawModelsAttribut_refc_ = nullptr;
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
	}
	if (emg_Buffer.DrawModelsCommands_refc_) {
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, emg_Buffer.index_comands_inderect_);
		glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
		emg_Buffer.DrawModelsCommands_refc_ = nullptr;
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, NULL);
	}
	if (emg_Buffer.DrawRegionOffsets_refc_) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, emg_Buffer.index_regions_offsets_);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		emg_Buffer.DrawRegionOffsets_refc_ = nullptr;
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
	}
	if (emg_Buffer.DrawRegionCommands_refc_) {
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, emg_Buffer.index_regions_command_);
		glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
		emg_Buffer.DrawRegionCommands_refc_ = nullptr;
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, NULL);
	}
	if (emg_Buffer.DrawRegionVertexs_refc_) {
		glBindBuffer(GL_ARRAY_BUFFER, emg_Buffer.index_regions_VBO_);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		emg_Buffer.DrawRegionVertexs_refc_ = nullptr;
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}
	if(emg_Buffer.index_meshes_VAO_) glDeleteVertexArrays(1, &emg_Buffer.index_meshes_VAO_);
	if(emg_Buffer.index_meshes_VBO_) glDeleteBuffers(1, &emg_Buffer.index_meshes_VBO_);
	if(emg_Buffer.index_comands_inderect_) glDeleteBuffers(1, &emg_Buffer.index_comands_inderect_);
	if(emg_Buffer.index_attribs_inderect_) glDeleteBuffers(1, &emg_Buffer.index_attribs_inderect_);
	if(emg_Buffer.index_matrixs_inderect_) glDeleteBuffers(1, &emg_Buffer.index_matrixs_inderect_);
	if(emg_Buffer.index_regions_VAO_) glDeleteVertexArrays(1, &emg_Buffer.index_regions_VAO_);
	if(emg_Buffer.index_regions_VBO_) glDeleteBuffers(1, &emg_Buffer.index_regions_VBO_);
	if(emg_Buffer.index_regions_command_) glDeleteBuffers(1, &emg_Buffer.index_regions_command_);
	if(emg_Buffer.index_regions_offsets_) glDeleteBuffers(1, &emg_Buffer.index_regions_offsets_);
	if(emg_Render.count_texutres_) glDeleteTextures(emg_Render.count_texutres_, emg_Buffer.textures_index_);
	if(emg_Buffer.index_camera_UBO_) glDeleteBuffers(1, &emg_Buffer.index_camera_UBO_);
	if(emg_Buffer.index_compute_shader_) glDeleteProgram(emg_Buffer.index_compute_shader_);
	if(emg_Buffer.index_draw_models_shader_) glDeleteProgram(emg_Buffer.index_draw_models_shader_);
	if(emg_Buffer.index_draw_region_shader_) glDeleteProgram(emg_Buffer.index_draw_region_shader_);
}
// Метод: Отрисовка сцены
__forceinline void emg::render::CreateFrame() {
	glFinish();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Очистка буфера цвета
	Interpolation_UpdateAttributes();//Интерполяция атрибутного массива
	glUseProgram(emg_Buffer.index_compute_shader_); //Включаем шейдер вычислений
	{	//Ждем пока отпустят буферы
		while (!emg_Render.draw_command_is_free_.exchange(false, std::memory_order_acquire)) {}; 
		//Передаем данные о вершинах, с проверкой на переполнение
		i_4b byte_size = emg_Render.count_instaces_ << 5;	// =  X * 32
		memcpy(emg_Buffer.DrawModelsAttribut_refc_, emg_Render.InstnAttribs_, byte_size);
		//Отслеживаем изменения в буфере команд отрисовкиs
		if (emg_Render.draw_command_update_.load(std::memory_order_acquire)) {
			byte_size = emg_Render.count_commands_ << 4;	// =  X * 16
			memcpy(emg_Buffer.DrawModelsCommands_refc_, emg_Render.DrawCommands_, byte_size);
		}
		emg_Render.draw_command_is_free_.store(true, std::memory_order_release);
	}
	glDispatchCompute(emg_Render.count_work_group_, 1, 1);			//Запуск расчетного шейдерa
	Interpolation_UpdateCameraData();//Обновить позицию камеры (считать ввод для камеры)
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);			//Проверка окончания работы расчетного шейдера
	glUseProgram(emg_Buffer.index_draw_models_shader_);				//После рсчета переходим к рендерингу
	{
		glBindBuffer(GL_UNIFORM_BUFFER, emg_Buffer.index_camera_UBO_);	//Захват буфера матрицы камеры
		glBufferSubData(GL_UNIFORM_BUFFER, 0, emg_Render.ViewMatrix_size_, emg_Render.ViewMatrix_refc_);//Передаем матрицу камеры
		glBindVertexArray(emg_Buffer.index_meshes_VAO_);				//Активация вершинного массива и массива команд
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, emg_Buffer.index_comands_inderect_);
		glActiveTexture(GL_TEXTURE0);									//Активация текстур
		glBindTexture(GL_TEXTURE_2D, emg_Buffer.textures_index_[0]);	//Используем текстуру
		glMultiDrawArraysIndirect(GL_TRIANGLES, 0, emg_Render.count_commands_, sizeof(mth::SDrawCommand)); //Отрисовка всех инстансов
	}
	//----------------------------------------------
}

#undef emg_Region
#undef emg_Buffer
#undef emg_Window
#undef emg_Render
#undef CHECK_3_LAST
#undef CHECK_3_FIRST
#undef CHECK_6_SYMBOLS_cmp
#undef CHECK_6_SYMBOLS_frg
#undef CHECK_6_SYMBOLS_vrt
#undef CHECK_6_SYMBOLS_msh
#undef CHECK_6_SYMBOLS_atl
#undef TEXTURES_LOAD_MODE
//===========================================================================================





//===========================================================================================
#define UnitVisualData emg::Data.Logic.unit_visual_data_
#define DRW_Commands emg::Data.Render.DrawCommands_
#define DRW_Attribs emg::Data.Render.InstnAttribs_
//Добавить сущность в рендер : emg::Data.Render.essence_add_to_show_id_
__forceinline void emg::render::InsertInDrawPipline() {
	mth::SEssenceMeta& ToShow_EntyData = UnitVisualData[emg::Data.Render.essence_add_to_show_id_];
	std::atomic<Bool>& IsFree = emg::Data.Render.draw_command_is_free_;
	std::atomic<Bool>& Update = emg::Data.Render.draw_command_update_;
	i_4b& InstCount = emg::Data.Render.count_instaces_;
	i_4b& CmndCount = emg::Data.Render.count_commands_;
	i_4b& WorkCount = emg::Data.Render.count_work_group_;

	if (InstCount >= ONLINE_LIMIT - 1) {
		Logger.SetError("ERROR::[ Render.h ]: InsertInDrawPipline()\n");
		Logger.SetError("\temg_Render.count_instaces_ >= ONLINE_LIMIT\n");
		Logger.SetError("\temg_Render.count_instaces_ = ");
		Logger.SetError(InstCount);
		Logger.SetError("\n");
		return;
	}
	while (!IsFree.exchange(false, std::memory_order_acquire)) { SLP_THRD(1); };
	//--------------[ Add to command ]--------------
	i_2b& mesh = ToShow_EntyData.mesh_id;
	for (i_4b i = mesh + 1; i < CmndCount; ++i) {
		DRW_Commands[i].inst_first += 1;
	}
	DRW_Commands[mesh].inst_count += 1;
	//------------[ Set then attribute ]------------
	i_2b inst = static_cast<i_2b>(DRW_Commands[mesh].inst_first);
	for (i_4b i = 0; i < ENTITY_LIMIT; ++i) {
		if (UnitVisualData[i].inst_id >= inst) {
			UnitVisualData[i].inst_id += 1;
		}
	}
	ToShow_EntyData.inst_id = static_cast<i_2b>(inst);
	//-------------[ Add to attribute ]-------------
	i_8b bytes_to_move = static_cast<i_8b>(InstCount - inst) << 5;
	if (bytes_to_move >= 0) {
		std::memmove(&DRW_Attribs[inst + 1], &DRW_Attribs[inst], bytes_to_move); 
	}
	else {
		Logger.SetError("ERROR::[ Render.h ]: InsertInDrawPipline()\n");
		Logger.SetError("\std::memmove(&DRW_Attribs[inst + 1], &DRW_Attribs[inst], bytes_to_move); << out of bytes_to_move \n");
	}
	DRW_Attribs[inst] = { 0 };
	DRW_Attribs[inst].ID_Instance = ToShow_EntyData.inst_id;
	DRW_Attribs[inst].ID_Material = ToShow_EntyData.txtr_id;
	InstCount += 1;
	WorkCount = (InstCount + 127) >> 7;
	//----------------------------------------------
	Update.store(true, std::memory_order_release);
	IsFree.store(true, std::memory_order_release);
}
//Удалить сущность из рендера : emg::Data.Render.essence_add_to_hide_id_
__forceinline void emg::render::RemoveInDrawPipline() {
	mth::SEssenceMeta& ToHide_EntyData = UnitVisualData[emg::Data.Render.essence_add_to_hide_id_];
	std::atomic<Bool>& IsFree = emg::Data.Render.draw_command_is_free_;
	std::atomic<Bool>& Update = emg::Data.Render.draw_command_update_;
	i_4b& WorkCount = emg::Data.Render.count_work_group_;
	i_4b& InstCount = emg::Data.Render.count_instaces_;
	i_4b& CmndCount = emg::Data.Render.count_commands_;

	if (InstCount <= 0) {
		Logger.SetError("ERROR::[ logic.h ]: Del_active_essence_to_render()\n");
		Logger.SetError("\temg_Render.count_instaces_ <= 0\n");
		Logger.SetError("\temg_Render.count_instaces_ = ");
		Logger.SetError(InstCount);
		Logger.SetError("\n");
		return;
	}
	i_2b& mesh = ToHide_EntyData.mesh_id;
	i_2b& inst = ToHide_EntyData.inst_id;
	if (DRW_Commands[mesh].inst_count < 1) {
		Logger.SetError("ERROR::[ logic.h ]: Del_active_essence_to_render()\n");
		Logger.SetError("\temg_CMD[mesh].inst_count < 1\n");
		Logger.SetError("\temg_CMD[mesh].inst_count = ");
		Logger.SetError(DRW_Commands[mesh].inst_count);
		Logger.SetError("\n");
		return;
	}
	//-----------[ Free then attribute ]------------
	ToHide_EntyData.inst_id = -1;
	for (u_4b i = 0; i < ENTITY_LIMIT; ++i) {
		if (UnitVisualData[i].inst_id >= static_cast<i_2b>(inst)) {
			UnitVisualData[i].inst_id -= 1;
		}
	}
	//--------------[ Del in command ]--------------
	while (!IsFree.exchange(false, std::memory_order_acquire)) { SLP_THRD(1); };
	DRW_Commands[mesh].inst_count -= 1;
	for (i_4b i = mesh + 1; i < CmndCount; ++i) {
		DRW_Commands[i].inst_first -= 1;
	}
	//-------------[ Del in attribute ]-------------
	i_8b bytes_to_move = static_cast<i_8b>(InstCount - inst) << 5;
	if (bytes_to_move >= 0) {
		std::memmove(&DRW_Attribs[inst], &DRW_Attribs[inst + 1], bytes_to_move);
	} else {
		Logger.SetError("ERROR::[ Render.h ]: RemoveInDrawPipline()\n");
		Logger.SetError("\std::memmove(&DRW_Attribs[inst], &DRW_Attribs[inst + 1], bytes_to_move); << out of bytes_to_move \n");
	}
	InstCount -= 1;
	WorkCount = (InstCount + 127) >> 7;
	//----------------------------------------------
	Update.store(true, std::memory_order_release);
	IsFree.store(true, std::memory_order_release);
}
#undef UnitVisualData 
#undef DRW_Commands
#undef DRW_Attribs
#undef COUT_CMD
//===========================================================================================





//===========================================================================================
#endif //MY_LIB_RENDER_REALISATION