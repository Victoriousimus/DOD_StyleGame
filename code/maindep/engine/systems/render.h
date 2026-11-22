#pragma once
#ifndef MY_LIB_RENDER_REALISATION
#define MY_LIB_RENDER_REALISATION
//===========================================================================================
#define FIRST_SYM(word, a, b, c) (word)[i] == (a) && (word)[i+1] == (b) && (word)[i+2] == (c)
#define LAST_SYM(word) (word)[i+3] == ':' && (word)[i+4] == '=' && (word)[i+5] == ' '
#define SYM_ModelCompShader(word) FIRST_SYM( (word), 'c', 'm', 'p') && LAST_SYM( (word) )
#define SYM_ModelFragShader(word) FIRST_SYM( (word), 'f', 'r', 'g') && LAST_SYM( (word) )
#define SYM_ModelVertShader(word) FIRST_SYM( (word), 'v', 'r', 't') && LAST_SYM( (word) )
#define SYM_ModelMeshData(word) FIRST_SYM( (word), 'm', 's', 'h') && LAST_SYM( (word) )
#define SYM_RegionVertShader(word) FIRST_SYM( (word), 'r', 'v', 't') && LAST_SYM( (word) )
#define SYM_RegionFragShader(word) FIRST_SYM( (word), 'r', 'f', 'g') && LAST_SYM( (word) )
#define SYM_RegionMeshData(word) FIRST_SYM( (word), 'm', 'a', 'p') && LAST_SYM( (word) )
#define SYM_TexturesAtlas(word) FIRST_SYM( (word), 'a', 't', 'l') && LAST_SYM( (word) )

#define emg_Window emg::Data.Window
#define emg_Render emg::Data.Render
#define emg_GPU emg_Render.GPU

#define DRW_Commands emg::Data.Render.DrawCommands_
#define DRW_Attribs emg::Data.Render.InstnAttribs_
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
		//DRW_Attribs[i].yaw += 0.01f;
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
	if (emg_Window.Input.key_flags_[EMG_INPUT_UP])   emg_Render.Camera.postn_.y += emg_Render.Camera.velocity_;
	if (emg_Window.Input.key_flags_[EMG_INPUT_DOWN]) emg_Render.Camera.postn_.y -= emg_Render.Camera.velocity_;
	//Обновим матрицу вида
	emg_Render.Camera.ang_front_.y = cosf(emg_Render.Camera.angle_.y);
	emg_Render.Camera.ang_front_.x *= emg_Render.Camera.ang_front_.y;
	emg_Render.Camera.ang_front_.z *= emg_Render.Camera.ang_front_.y;
	emg_Render.Camera.ang_front_.y = sinf(emg_Render.Camera.angle_.y);
	emg_Render.Camera.ang_front_ = glm::normalize(emg_Render.Camera.ang_front_);
	emg_Render.Camera.view_matrix_ = emg_Render.Camera.projection_matrix_ *
		glm::lookAt( emg_Render.Camera.postn_, emg_Render.Camera.postn_ + emg_Render.Camera.ang_front_, emg_Render.Camera.null_view_ );
}

// Процедура формирования шейдерного буфера с прямой ссылкой на память
__forceinline PMem bild_refc_storage(u_4b* buf_id, i_8b byte_size, i_4b layout, u_4b type, u_4b flags) {
	PMem buf_refc = nullptr;
	glGenBuffers(1, buf_id);
	glBindBuffer(type, *buf_id);
	glBufferStorage(type, byte_size, nullptr, flags);
	buf_refc = glMapBufferRange(type, 0, byte_size, flags);
	if (layout > 0) {
		u_4b ShaderLayout = layout;
		glBindBufferBase(type, ShaderLayout, *buf_id); // Помечаем как RegionOffsets
	}
	glBindBuffer(type, NULL);
	return buf_refc;
}
// Построение массивов аттрибутов и вершин регионов
__forceinline f_4b* bild_region(i_8b* array_size, i_4b quad_w, f_4b stride_c) {
	*array_size = 0;
	const i_8b QUAD_W = quad_w;
	const f_4b COORDINATE_STRIDE = stride_c;
	const i_8b floatCount = QUAD_W * QUAD_W * 24;
	f_4b* Array = new f_4b[floatCount];
	for (i_8b y = 0; y < QUAD_W; ++y) {
		for (i_8b x = 0; x < QUAD_W; ++x) {
			const i_8b p = (y * QUAD_W + x) * 24;
			const f_4b x0 = static_cast<f_4b>(x + 0) * COORDINATE_STRIDE;
			const f_4b y0 = static_cast<f_4b>(y + 0) * COORDINATE_STRIDE;
			const f_4b x1 = static_cast<f_4b>(x + 1) * COORDINATE_STRIDE;
			const f_4b y1 = static_cast<f_4b>(y + 1) * COORDINATE_STRIDE;
			const f_4b u0 = 0.5f, u1 = 0.9f;
			const f_4b v0 = 0.1f, v1 = 0.5f;
			// Треугольник 1:
			Array[p + 0] = x0;	Array[p + 1] = y0;	Array[p + 2] = u0; Array[p + 3] = v0;
			Array[p + 4] = x0;	Array[p + 5] = y1;	Array[p + 6] = u0; Array[p + 7] = v1;
			Array[p + 8] = x1;	Array[p + 9] = y1;	Array[p + 10] = u1; Array[p + 11] = v1;
			// Треугольник 2:
			Array[p + 12] = x1; Array[p + 13] = y0;	Array[p + 14] = u1; Array[p + 15] = v0;
			Array[p + 16] = x0; Array[p + 17] = y0;	Array[p + 18] = u0; Array[p + 19] = v0;
			Array[p + 20] = x1; Array[p + 21] = y1;	Array[p + 22] = u1; Array[p + 23] = v1;
		}
	}
	*array_size = floatCount;
	return Array;
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
		if (SYM_ModelCompShader(fileText)) {
			shader_type = GL_COMPUTE_SHADER;
			shader_ref = &shdr_Cmp;
		}
		else
		if (SYM_ModelFragShader(fileText)) {
			shader_type = GL_FRAGMENT_SHADER;
			shader_ref = &shdr_m_Frg;
		}
		else
		if (SYM_ModelVertShader(fileText)) {
			shader_type = GL_VERTEX_SHADER;
			shader_ref = &shdr_m_Vrt;
		}
		else
		if (SYM_RegionFragShader(fileText)) {
			shader_type = GL_FRAGMENT_SHADER;
			shader_ref = &shdr_r_Frg;
		}
		else
		if (SYM_RegionVertShader(fileText)) {
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
// Загрузка массивов аттрибутов и вершин моделей
__forceinline void load_all_vetex_data(f_4b** result_array, i_8b* array_size, i_4b* models_count) {
	f_4b* all_VertexAttribsArray;
	f_4b** vecArr = nullptr;	// указатель на массивы вершин
	u_8b* vecLen = nullptr;		// указатели на длинны массивов
	u_8b lengt_accum = 0;
	i_4b model_count = 0;
	t_wrd fileNames[MODELS_LIMIT] = {0};
	t_wrd fileText;
	*result_array = nullptr;
	*models_count = 0;
	*array_size = 0;

	fileText = iof::load_file_txt(emg::Data.Files.files_data_map_, &lengt_accum);
	if (!fileText) {
		Logger.SetError("ERROR::[ render.h ]: MoDels_List load [");
		Logger.SetError(emg::Data.Files.files_data_map_);
		Logger.SetError(" ] \n");
		*result_array = nullptr;
		*models_count = 0;
		*array_size = 0;
		return;
	}
	for (i_4b j, i = 0; i < lengt_accum - 5; ++i) {
		if(SYM_ModelMeshData(fileText)) {
			fileNames[model_count] = new t_sym[64];
			if (!fileNames[model_count]) {
				Logger.SetError("ERROR::[ render.h ]: load_all_vetex_data(result_array, array_size): fileNames[");
				Logger.SetError(model_count);
				Logger.SetError("] MALLOC ERROR\n");
				for (j = 0; j < model_count; ++j) { delete[] fileNames[j]; }
				free(fileText);
				*result_array = nullptr;
				*models_count = 0;
				*array_size = 0;
				return;
			}
			for (j = 0, i += 6; i < lengt_accum; ++i, ++j) {
				fileNames[model_count][j] = fileText[i];
				if (fileNames[model_count][j] == '*') break;
			}
			fileNames[model_count][j] = '\0';
			++model_count;
		}
	} 
	free(fileText);
	// Инициализируем временное хранилище памяти для штучных мешей, для дальнейшего объеденения
	vecArr = new f_4b*[model_count];	// <- Массив указателей на масссивы вершин
	vecLen = new u_8b[model_count];	// <- Массив длин массивов по указателю
	if (!vecArr || !vecLen) {
		Logger.SetError("ERROR::[ render.h ]:vecArr = malloc(model_count * size); vecLen = malloc(model_count * size)\n");
		for (i_4b i = 0; i < model_count; ++i) { delete[] fileNames[i]; }
		if(vecLen) delete[] vecLen;
		if(vecArr) delete[] vecArr;
		*result_array = nullptr;
		*models_count = 0;
		*array_size = 0;
		return;
	}
	lengt_accum = 0;
	for (i_4b i = 0; i < model_count; ++i) { vecArr[i] = nullptr; }
	// Загружаем во вершины в массивы и формируем под него DrawComand
	COUT_CMD( std::cout << "--------------------------------------------------\n" << std::endl );
	for (i_4b i = 0; i < model_count; ++i) {
		vecArr[i] = static_cast<f_4b*>(iof::load_file_ultrafast(fileNames[i], &vecLen[i]));// vecLen[i] - будет записана длинна массива в байтах
		delete[] fileNames[i];
		if (!vecArr[i]) {
			Logger.SetError("ERROR::[ render.h ]:vecArr[i] = static_cast<f_4b*>(iof::load_file_ultrafast(fileNames[i], &vertx_count));\n");
			// Подчищаем временные хранилища мешей
			for (i_4b j = i; j > -1; --j) { iof::free_ultrafast_loaded_data(vecArr[j]); }
			for (i_4b j = 0; j < model_count; ++j) { delete[] fileNames[j]; }
			delete[] vecLen;
			delete[] vecArr;
			*result_array = nullptr; 
			*models_count = 0;
			*array_size = 0;
			return;
		}
		DRW_Commands[i].inst_count = 0; // Количество инстансов
		DRW_Commands[i].vert_count = static_cast<u_4b>(vecLen[i]) >> 5;	// Количество вершин для примитива
		DRW_Commands[i].inst_first = i < 1 ? 0 : // Базовый инстанс (добавляется к gl_instance_index)
			DRW_Commands[i - 1].inst_first +
			DRW_Commands[i - 1].inst_count;
		DRW_Commands[i].vert_first = i < 1 ? 0 :	// Начальный индекс в вершинном массиве
			DRW_Commands[i - 1].vert_first +
			DRW_Commands[i - 1].vert_count;
		vecLen[i] = static_cast<u_4b>(vecLen[i] >> 2);			// Сдвиг чтобы сичтать f_4b
		lengt_accum += vecLen[i];
		COUT_CMD(
				std::cout
				<< "------------------------------------\n"
				<< "[ inst_first : " << DRW_Commands[i].inst_first << " ]; "
				<< "[ inst_count : " << DRW_Commands[i].inst_count << " ]; "
				<< "[ vert_first : " << DRW_Commands[i].vert_first << " ]; "
				<< "[ vert_count : " << DRW_Commands[i].vert_count << " ]; "
				<< "------------------------------------\n"
				<< std::endl
			);
	}
	COUT_CMD( std::cout << "--------------------------------------------------\n" << std::endl );
	all_VertexAttribsArray = static_cast<f_4b*>(malloc(lengt_accum * sizeof(f_4b)));
	if (!all_VertexAttribsArray) {
		Logger.SetError("ERROR::[ render.h ]: static_cast<f_4b*>(malloc(lengt_accum * sizeof(f_4b)))\n");
		for (i_4b i = 0; i < model_count;++i) { iof::free_ultrafast_loaded_data(vecArr[i]); }
		delete[] vecLen;
		delete[] vecArr;
		*result_array = nullptr;
		*models_count = 0;
		*array_size = 0;
		return;
	};
	for (i_4b OnVec = 0, p = 0, i = 0; i < lengt_accum; ++i, ++p) {
		if (p >= vecLen[OnVec]) {
			iof::free_ultrafast_loaded_data(vecArr[OnVec]);
			p = 0; ++OnVec;
		}
		all_VertexAttribsArray[i] = (vecArr[OnVec])[p];
	}
	delete[] vecLen;
	delete[] vecArr;
	*array_size = lengt_accum;
	*models_count = model_count;
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

	emg_Render.draw_command_update_.store(false); // Массив команд отрисовки изменен
	emg_Render.draw_command_is_free_.store(true); // Массив команд отрисовки изменен
	emg_Render.count_work_group_ = 0;
	emg_Render.count_instaces_ = 0;
	emg_Render.count_commands_ = 0;
	for (u_4b i = 0; i < ONLINE_LIMIT; ++i) {
		DRW_Attribs[i].pich = 0; //Тангаж
		DRW_Attribs[i].yaw = 0; //Рысканье
		DRW_Attribs[i].roll = 0; //Крен
		DRW_Attribs[i].x = 0; //положительные = влево
		DRW_Attribs[i].y = 0; //положительные = дальш
		DRW_Attribs[i].z = 0; //положительные = выше
	}
}
// Init() < - Mассив трансформаций инстансов: attrib_ptr_draw[]. Заполняет матрицы: mProj, mView, mSpace
__forceinline void init_view_matrix() {
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
	emg_Render.Camera.view_size_ = sizeof(m4_64b);
	emg_Render.Camera.view_refc_ = glm::value_ptr(emg_Render.Camera.view_matrix_);
	emg_Render.Camera.view_matrix_ = m4_64b(1) * emg_Render.Camera.projection_matrix_;
	{//+++++++++++++++++[ Camera Buffer ]++++++++++++++++++
		glGenBuffers(1, &emg_GPU.index_camera_UBO_);															// Создание буфера
		glBindBuffer(GL_UNIFORM_BUFFER, emg_GPU.index_camera_UBO_);											// Захват буфера
		glBufferData(GL_UNIFORM_BUFFER, emg_Render.Camera.view_size_, nullptr, GL_DYNAMIC_DRAW);				// Настройка буфера
		glBindBufferBase(GL_UNIFORM_BUFFER, emg_GPU.Layout::VwProj, emg_GPU.index_camera_UBO_);			// Помечаем как ViewProj
		glBufferSubData(GL_UNIFORM_BUFFER, NULL, emg_Render.Camera.view_size_, emg_Render.Camera.view_refc_);	// Загрузка буфера
		glBindBuffer(GL_UNIFORM_BUFFER, NULL);																	// Отпускаем буффер
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
	emg_GPU.index_compute_shader_ = glCreateProgram();
	glAttachShader(emg_GPU.index_compute_shader_, Prog.compute_Shader);
	glLinkProgram(emg_GPU.index_compute_shader_);
	glGetProgramiv(emg_GPU.index_compute_shader_, GL_LINK_STATUS, &Prog.success_loading);
	if (!Prog.success_loading) {
		glDeleteShader(Prog.compute_Shader);
		glDeleteShader(Prog.models_VertShader);
		glDeleteShader(Prog.models_FragShader);
		char infoLog[512];
		glGetProgramInfoLog(emg_GPU.index_compute_shader_, 512, NULL, infoLog);
		Logger.SetError("ERROR::SHADER::PROGRAM::LINKING_FAILED [COMPUTE SHADER]  LOG:\n");
		Logger.SetError(infoLog);
		Logger.SetError("\n");
		return;
	}
	// Создание шейдерной программы и линковка программы для объектов
	emg_GPU.index_draw_models_shader_ = glCreateProgram();
	glAttachShader(emg_GPU.index_draw_models_shader_, Prog.models_VertShader);
	glAttachShader(emg_GPU.index_draw_models_shader_, Prog.models_FragShader);
	glLinkProgram(emg_GPU.index_draw_models_shader_);
	glGetProgramiv(emg_GPU.index_draw_models_shader_, GL_LINK_STATUS, &Prog.success_loading);
	if (!Prog.success_loading) {
		glDeleteShader(Prog.compute_Shader);
		glDeleteShader(Prog.models_VertShader);
		glDeleteShader(Prog.models_FragShader);
		char infoLog[1024];
		glGetProgramInfoLog(emg_GPU.index_draw_models_shader_, 1024, NULL, infoLog);
		Logger.SetError("ERROR::SHADER::PROGRAM::LINKING_FAILED [ BASE_DRAW_SHADER ]  LOG:\n");
		Logger.SetError(infoLog);
		Logger.SetError("\n");
		return;
	}
	// Создание шейдерной программы и линковка программы для местности
	emg_GPU.index_draw_region_shader_ = glCreateProgram();
	glAttachShader(emg_GPU.index_draw_region_shader_, Prog.region_VertShader);
	glAttachShader(emg_GPU.index_draw_region_shader_, Prog.region_FragShader);
	glLinkProgram(emg_GPU.index_draw_region_shader_);
	glGetProgramiv(emg_GPU.index_draw_region_shader_, GL_LINK_STATUS, &Prog.success_loading);
	if (!Prog.success_loading) {
		glDeleteShader(Prog.compute_Shader);
		glDeleteShader(Prog.models_VertShader);
		glDeleteShader(Prog.models_FragShader);
		glDeleteShader(Prog.region_VertShader);
		glDeleteShader(Prog.region_FragShader);
		char infoLog[1024];
		glGetProgramInfoLog(emg_GPU.index_draw_region_shader_, 1024, NULL, infoLog);
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
// Init() < - Заполняет загружает и передает модели в матрицу [Важен порядок загрузки в "data_map.mem"]
__forceinline void load_mesh_to_GPU() {
	i_8b bufferByteSize;
	{//+++++++++++++++++[ Vertex Buffer ]++++++++++++++++++
		i_8b float_count;
		f_4b* All_ModelsVertexes;
		load_all_vetex_data(&All_ModelsVertexes, &float_count, &emg_Render.count_commands_);
		if (!All_ModelsVertexes || float_count < 24) {
			Logger.SetError("ERROR::[render.h]: MODELS_LOAD_FAIL [ float_count < 24 ]\n");
			return;
		}
		bufferByteSize = float_count * sizeof(f_4b);
		glGenVertexArrays(1, &emg_GPU.index_meshes_VAO_);
		glBindVertexArray(emg_GPU.index_meshes_VAO_);
		glGenBuffers(1, &emg_GPU.index_meshes_VBO_);
		glBindBuffer(GL_ARRAY_BUFFER, emg_GPU.index_meshes_VBO_);
		glBufferStorage(GL_ARRAY_BUFFER, float_count * sizeof(f_4b), All_ModelsVertexes, NULL);
		glVertexAttribPointer(emg_GPU.Layout::MshPos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f_4b), (PMem)0);
		glEnableVertexAttribArray(emg_GPU.Layout::MshPos); // Координаты
		glVertexAttribPointer(emg_GPU.Layout::MshNrm, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f_4b), (PMem)(3 * sizeof(f_4b)));
		glEnableVertexAttribArray(emg_GPU.Layout::MshNrm); // Нормали
		glVertexAttribPointer(emg_GPU.Layout::MshTex, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f_4b), (PMem)(6 * sizeof(f_4b)));
		glEnableVertexAttribArray(emg_GPU.Layout::MshTex); // Текстура
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray(NULL);
		free(All_ModelsVertexes);
		u_8b error_code = glGetError();
		if (error_code != GL_NO_ERROR) {
			Logger.SetError("ERROR::[render.h]: MODEL_BUFFER_FAIL \"emg_GPU.index_meshes_VAO_\" or \"emg_GPU.index_meshes_VBO_\"\n");
			Logger.SetError("GL_ERROR :"); Logger.SetError(error_code); Logger.SetError("\n");
		}
	}
	{//+++++++++++++++++[ Matrix Buffer ]++++++++++++++++++
		bufferByteSize = static_cast<i_8b>(ONLINE_LIMIT) * sizeof(m4_64b);
		glGenBuffers(1, &emg_GPU.index_meshes_matrix_);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, emg_GPU.index_meshes_matrix_);
		glBufferStorage(GL_SHADER_STORAGE_BUFFER, bufferByteSize, NULL, NULL);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, emg_GPU.Layout::Matrix, emg_GPU.index_meshes_matrix_);// Помечаем как матрицы
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
		u_8b error_code = glGetError();
		if (error_code != GL_NO_ERROR) {
			Logger.SetError("ERROR::[render.h]: MODEL_BUFFER_FAIL \"emg_GPU.index_meshes_matrix_\"\n");
			Logger.SetError("GL_ERROR :"); Logger.SetError(error_code); Logger.SetError("\n");
		}
	}
	{//+++++++++++++++++[ Attrib Buffer ]++++++++++++++++++
		bufferByteSize = static_cast<i_8b>(ONLINE_LIMIT) * sizeof(mth::SInstnAttrib);
		u_4b type = GL_SHADER_STORAGE_BUFFER, flag = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		emg_GPU.DrawModelsAttribut_refc_ = bild_refc_storage(&emg_GPU.index_attribs_inderect_, bufferByteSize, emg_GPU.Layout::Attrib, type, flag);
		if (!emg_GPU.DrawModelsAttribut_refc_) Logger.SetError("ERROR::[ render.h ]: MODELS_MAPING_FAIL &<-DrawModelsAttribut_refc_\n");
		u_8b error_code = glGetError();
		if (error_code != GL_NO_ERROR) {
			Logger.SetError("ERROR::[render.h]: MODEL_BUFFER_FAIL \"emg_GPU.index_attribs_inderect_\"\n");
			Logger.SetError("GL_ERROR :"); Logger.SetError(error_code); Logger.SetError("\n");
		}
	}
	{//+++++++++++++++++[ Command Buffer ]+++++++++++++++++
		bufferByteSize = static_cast<i_8b>(MODELS_LIMIT) * sizeof(mth::SDrawCommand);
		u_4b type = GL_DRAW_INDIRECT_BUFFER, flag = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		emg_GPU.DrawModelsCommands_refc_ = bild_refc_storage(&emg_GPU.index_comands_inderect_, bufferByteSize, -1, type, flag);
		if (!emg_GPU.DrawModelsCommands_refc_) Logger.SetError("ERROR::[ render.h ]: MODELS_MAPING_FAIL &<-DrawModelsCommands_refc_\n");
		u_8b error_code = glGetError();
		if (error_code != GL_NO_ERROR) {
			Logger.SetError("ERROR::[render.h]: MODEL_BUFFER_FAIL \"emg_GPU.index_comands_inderect_\"\n");
			Logger.SetError("GL_ERROR :"); Logger.SetError(error_code); Logger.SetError("\n");
		}
	}
}
// Init() < - Подготавливает буфер для мешей террейна
__forceinline void init_wrld_in_GPU() {
	i_8b bufferByteSize{};
	emg_GPU.DrawRegionCommands_refc_ = nullptr;
	emg_GPU.DrawRegionOffsets_refc_ = nullptr;
	{//+++++++++++++++++[ Vertex Buffer ]++++++++++++++++++
		const i_4b LOD_COUNT = REGION_COMND_COUNT;
		const i_4b MAP_MAXIMAL = 128;
		i_8b full_size = 0;
		i_8b  LOD_size[LOD_COUNT]{ 0 };
		f_4b* LOD_data[LOD_COUNT]{ 0 };
		for (i_4b i = 0; i < LOD_COUNT; ++i) {
			i_4b size = MAP_MAXIMAL >> i;
			if (size < 1) {
				Logger.SetError("ERROR::[render.h]: LENDSPACE_BILD_FAIL [ LOD_");
				Logger.SetError(i); Logger.SetError(" <-> SIZE < 1 ]\n");
			}
			LOD_data[i] = bild_region(&LOD_size[i], size, (1.0f * static_cast<f_4b>(1<<i)));
			full_size += LOD_size[i];
			if (!LOD_data[i] || LOD_size[i] < 24) {
				Logger.SetError("ERROR::[render.h]: LENDSPACE_BILD_FAIL [ LOD_");
				Logger.SetError(i); Logger.SetError(" < 24 ]\n");
			}
			if (Logger.error_flag_ < 0) { for (i_4b j = i; j > 0; --i) { delete[] LOD_data[j]; } return; }
		}
		f_4b* all_VertexArray = new f_4b[full_size];
		if (!all_VertexArray || full_size < 24) {
			Logger.SetError("ERROR::[render.h]: LENDSPACE_FAIL_ALLOC [ FullArray < 24 ]\n");
			for (i_4b i = 0; i < LOD_COUNT; ++i) delete[] LOD_data[i];
			return;
		}
		for (i_8b s = 0, p = 0, i = 0; i < full_size; ++i) {
			if (p >= LOD_size[s]) {
				delete[] LOD_data[s];
				if ( ++s >= LOD_COUNT ) {
					Logger.SetError("ERROR::[render.h]: LENDSPACE OUT OF RANGE\n");
					for (i_8b j = s; j < LOD_COUNT; ++j) {
						delete[] LOD_data[j];
					}
					delete[] all_VertexArray;
					return;
				}
				p = 0;
			}
			all_VertexArray[i] = LOD_data[s][p];
			++p;
		}

		for (i_8b i = 0; i < LOD_COUNT; ++i) {
			emg_Render.RegionCommands_[i].inst_first = i > 0 ?
				emg_Render.RegionCommands_[i - 1].inst_count +
				emg_Render.RegionCommands_[i - 1].inst_first : 0;
			emg_Render.RegionCommands_[i].vert_first = i > 0 ?
				emg_Render.RegionCommands_[i - 1].vert_count + 
				emg_Render.RegionCommands_[i - 1].vert_first : 0;
			emg_Render.RegionCommands_[i].inst_count = 1;
			emg_Render.RegionCommands_[i].vert_count = static_cast<u_4b>(LOD_size[i] >> 2);
			emg_Render.RegionOffsets_[i].x = static_cast<f_4b>(MAP_MAXIMAL * i);
			emg_Render.RegionOffsets_[i].y = 0.0f;
		}



		bufferByteSize = full_size * sizeof(f_4b);
		glGenVertexArrays(1, &emg_GPU.index_regions_VAO_);
		glBindVertexArray(emg_GPU.index_regions_VAO_);
		glGenBuffers(1, &emg_GPU.index_regions_VBO_);
		glBindBuffer(GL_ARRAY_BUFFER, emg_GPU.index_regions_VBO_);
		glBufferStorage(GL_ARRAY_BUFFER, bufferByteSize, all_VertexArray, NULL);
		glVertexAttribPointer(emg_GPU.Layout::RegPos, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f_4b), (PMem)0);
		glEnableVertexAttribArray(emg_GPU.Layout::RegPos); // Координаты
		glVertexAttribPointer(emg_GPU.Layout::RegTex, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f_4b), (PMem)(2 * sizeof(f_4b)));
		glEnableVertexAttribArray(emg_GPU.Layout::RegTex); // Текстура
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray(NULL);
		delete[] all_VertexArray;
		u_8b error_code = glGetError();
		if (error_code != GL_NO_ERROR) {
			Logger.SetError("ERROR::[render.h]: LENDSPACE_BUFFER_FAIL \"emg_GPU.index_regions_VAO_\" or \"emg_GPU.index_regions_VBO_\"\n");
			Logger.SetError("GL_ERROR :"); Logger.SetError(error_code); Logger.SetError("\n");
		}
	}
	{//+++++++++++++++++[ Command Buffer ]+++++++++++++++++
		bufferByteSize = static_cast<i_8b>(REGION_COMND_COUNT) * 16;
		u_4b type = GL_DRAW_INDIRECT_BUFFER, flag = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		emg_GPU.DrawRegionCommands_refc_ = bild_refc_storage(&emg_GPU.index_regions_command_, bufferByteSize, -1, type, flag);
		if (!emg_GPU.DrawRegionCommands_refc_) Logger.SetError("ERROR::[render.h]: LENDSPACE_UNMAPPED [emg_GPU.DrawRegionCommands_refc_]\n");
		else memcpy(emg_GPU.DrawRegionCommands_refc_, emg_Render.RegionCommands_, bufferByteSize);
		u_8b error_code = glGetError();
		if (error_code != GL_NO_ERROR) {
			Logger.SetError("ERROR::[render.h]: LENDSPACE_BUFFER_FAIL \"emg_GPU.index_regions_command_\"\n");
			Logger.SetError("GL_ERROR :"); Logger.SetError(error_code); Logger.SetError("\n");
		}
	}
	{//+++++++++++++++++[ Offsets Buffer ]+++++++++++++++++
		bufferByteSize = static_cast<i_8b>(REGION_DRAWS_COUNT) * 8;
		u_4b type = GL_SHADER_STORAGE_BUFFER, flag = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		emg_GPU.DrawRegionOffsets_refc_ = bild_refc_storage(&emg_GPU.index_regions_offsets_, bufferByteSize, emg_GPU.Layout::Offset, type, flag);
		if (!emg_GPU.DrawRegionOffsets_refc_)  Logger.SetError("ERROR::[render.h]: LENDSPACE_UNMAPPED [emg_GPU.DrawRegionOffsets_refc_]\n");
		else memcpy(emg_GPU.DrawRegionOffsets_refc_, emg_Render.RegionOffsets_, bufferByteSize);
		u_8b error_code = glGetError();
		if (error_code != GL_NO_ERROR) {
			Logger.SetError("ERROR::[render.h]: LENDSPACE_BUFFER_FAIL \"emg_GPU.index_regions_offsets_\"\n");
			Logger.SetError("GL_ERROR :"); Logger.SetError(error_code); Logger.SetError("\n");
		}
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
		if (SYM_TexturesAtlas(fileText)) {
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
	glGenTextures(emg_Render.count_texutres_, emg_GPU.textures_index_);
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
		glBindTexture(GL_TEXTURE_2D, emg_GPU.textures_index_[i]);
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
	load_txtr_to_GPU(); if (Logger.error_flag_ < 0) return;
	load_mesh_to_GPU(); if (Logger.error_flag_ < 0) return;
	init_wrld_in_GPU(); if (Logger.error_flag_ < 0) return;
}
// Метод: Oчистка ресурсов
__forceinline void emg::render::Exit() {
	// Дождаться завершения всех операций GPU
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glUseProgram(NULL);
	glFinish();
	// Отчистка ресурсов    
	if (emg_GPU.DrawModelsAttribut_refc_) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, emg_GPU.index_attribs_inderect_);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		emg_GPU.DrawModelsAttribut_refc_ = nullptr;
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
	}
	if (emg_GPU.DrawModelsCommands_refc_) {
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, emg_GPU.index_comands_inderect_);
		glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
		emg_GPU.DrawModelsCommands_refc_ = nullptr;
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, NULL);
	}
	if (emg_GPU.DrawRegionOffsets_refc_) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, emg_GPU.index_regions_offsets_);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		emg_GPU.DrawRegionOffsets_refc_ = nullptr;
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
	}
	if (emg_GPU.DrawRegionCommands_refc_) {
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, emg_GPU.index_regions_command_);
		glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
		emg_GPU.DrawRegionCommands_refc_ = nullptr;
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, NULL);
	}
	glDeleteVertexArrays(1, &emg_GPU.index_meshes_VAO_);
	glDeleteBuffers(1, &emg_GPU.index_meshes_VBO_);
	glDeleteBuffers(1, &emg_GPU.index_comands_inderect_);
	glDeleteBuffers(1, &emg_GPU.index_attribs_inderect_);
	glDeleteVertexArrays(1, &emg_GPU.index_regions_VAO_);
	glDeleteBuffers(1, &emg_GPU.index_regions_VBO_);
	glDeleteBuffers(1, &emg_GPU.index_regions_command_);
	glDeleteBuffers(1, &emg_GPU.index_regions_offsets_);
	glDeleteTextures(emg_Render.count_texutres_, emg_GPU.textures_index_);
	glDeleteBuffers(1, &emg_GPU.index_camera_UBO_);
	glDeleteBuffers(1, &emg_GPU.index_meshes_matrix_);
	glDeleteProgram(emg_GPU.index_compute_shader_);
	glDeleteProgram(emg_GPU.index_draw_models_shader_);
	glDeleteProgram(emg_GPU.index_draw_region_shader_);
}
// Метод: Отрисовка сцены
__forceinline void emg::render::CreateFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Очистка буфера цвета
	Interpolation_UpdateCameraData();//Обновить позицию камеры (считать ввод для камеры)
	Interpolation_UpdateLendspaces();//Интерполяция атрибутного массива
	Interpolation_UpdateAttributes();//Интерполяция атрибутного массива
	glUseProgram(emg_GPU.index_compute_shader_); //Включаем шейдер вычислений
	{	//Ждем пока отпустят буферы
		while (!emg_Render.draw_command_is_free_.exchange(false, std::memory_order_acquire)) {};
		//Передаем данные о вершинах, с проверкой на переполнение
		u_8b ByteSize = static_cast<u_8b>(emg_Render.count_instaces_) << 5;	// =  X * 32
		memcpy(emg_GPU.DrawModelsAttribut_refc_, DRW_Attribs, ByteSize);
		//Отслеживаем изменения в буфере команд отрисовкиs
		if (emg_Render.draw_command_update_.load(std::memory_order_acquire)) {
			ByteSize = static_cast<u_8b>(emg_Render.count_commands_) << 4;	// =  X * 16
			memcpy(emg_GPU.DrawModelsCommands_refc_, DRW_Commands, ByteSize);
		}
		emg_Render.draw_command_is_free_.store(true, std::memory_order_release);
		glDispatchCompute(emg_Render.count_work_group_, 1, 1);	//Запуск расчетного шейдерa
		glActiveTexture(GL_TEXTURE0);									//Активация текстур
		glBindTexture(GL_TEXTURE_2D, emg_GPU.textures_index_[0]);	//Используем текстуру
		glBindBuffer(GL_UNIFORM_BUFFER, emg_GPU.index_camera_UBO_);	//Захват буфера матрицы камеры
		glBufferSubData(GL_UNIFORM_BUFFER, 0, emg_Render.Camera.view_size_, emg_Render.Camera.view_refc_);//Передаем матрицу камеры
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);			//Проверка окончания работы расчетного шейдера
	}
	glUseProgram(emg_GPU.index_draw_region_shader_); // При использовании этой программы значительно просидает ФПС!!!!!!!!!!!!!!!!!
	{ // Начало работы с шейдером отрисовки регионов
		glBindVertexArray(emg_GPU.index_regions_VAO_);				//Активация вершинного массива и массива команд
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, emg_GPU.index_regions_command_);
		glMultiDrawArraysIndirect(GL_TRIANGLES, 0, REGION_COMND_COUNT, sizeof(mth::SDrawCommand)); //Отрисовка всех инстансов
	} //Конец работы с шейдером отрисовки регионов
	glUseProgram(emg_GPU.index_draw_models_shader_);				//После рсчета переходим к рендерингу
	{
		glBindVertexArray(emg_GPU.index_meshes_VAO_);				//Активация вершинного массива и массива команд
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, emg_GPU.index_comands_inderect_);
		glMultiDrawArraysIndirect(GL_TRIANGLES, 0, emg_Render.count_commands_, sizeof(mth::SDrawCommand)); //Отрисовка всех инстансов
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);	//Деактивация
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, NULL);	//Деактивация
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);			//Деактивация
	glBindVertexArray(NULL);						//Деактивация
	//----------------------------------------------
}

#undef emg_GPU
#undef emg_Window
#undef emg_Render
#undef SYM_ModelCompShader
#undef SYM_ModelFragShader
#undef SYM_ModelVertShader
#undef SYM_ModelMeshData
#undef SYM_RegionVertShader
#undef SYM_RegionFragShader
#undef SYM_RegionMeshData
#undef SYM_TexturesAtlas
#undef LAST_SYM
#undef FIRST_SYM
#undef TEXTURES_LOAD_MODE
//===========================================================================================
#define UnitVisualData emg::Data.Logic.unit_visual_data_
//Добавить сущность в рендер : emg::Data.Render.essence_add_to_show_id_
__forceinline void emg::render::InsertInDrawPipline(u_2b entityID) {
	mth::SEssenceMeta& ToShow_EntyData = UnitVisualData[entityID];
	std::atomic<Bool>& IsFree = emg::Data.Render.draw_command_is_free_;
	std::atomic<Bool>& Update = emg::Data.Render.draw_command_update_;
	i_4b& InstCount = emg::Data.Render.count_instaces_;
	i_4b& CmndCount = emg::Data.Render.count_commands_;
	i_4b& WorkCount = emg::Data.Render.count_work_group_;

	if (InstCount >= ONLINE_LIMIT - 1) {
		Logger.SetError("ERROR::[ render.h ]: add_active_essence_to_render()\n");
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
		if (UnitVisualData[i].inst_id < inst) continue;
		UnitVisualData[i].inst_id += 1;
	}
	ToShow_EntyData.inst_id = inst;
	//-------------[ Add to attribute ]-------------
	i_8b bytes_to_move = static_cast<i_8b>(InstCount - inst) << 5;
	if (bytes_to_move > 0) {
		std::memmove(&DRW_Attribs[inst + 1], &DRW_Attribs[inst], bytes_to_move); 
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
__forceinline void emg::render::RemoveInDrawPipline(u_2b entityID) {
	mth::SEssenceMeta& ToHide_EntyData = UnitVisualData[entityID];
	std::atomic<Bool>& IsFree = emg::Data.Render.draw_command_is_free_;
	std::atomic<Bool>& Update = emg::Data.Render.draw_command_update_;
	i_4b& WorkCount = emg::Data.Render.count_work_group_;
	i_4b& InstCount = emg::Data.Render.count_instaces_;
	i_4b& CmndCount = emg::Data.Render.count_commands_;

	if (InstCount <= 0) {
		Logger.SetError("ERROR::[ render.h ]: RemoveInDrawPipline()\n");
		Logger.SetError("\temg_Render.count_instaces_ <= 0\n");
		Logger.SetError("\temg_Render.count_instaces_ = ");
		Logger.SetError(InstCount);
		Logger.SetError("\n");
		return;
	}
	i_2b& mesh = ToHide_EntyData.mesh_id;
	i_2b& inst = ToHide_EntyData.inst_id;
	if (DRW_Commands[mesh].inst_count < 1) {
		Logger.SetError("ERROR::[ render.h ]: RemoveInDrawPipline()\n");
		Logger.SetError("\tDRW_Commands[mesh].inst_count < 1\n");
		Logger.SetError("\tDRW_Commands[mesh].inst_count = ");
		Logger.SetError(DRW_Commands[mesh].inst_count);
		Logger.SetError("\n");
		return;
	}
	//-----------[ Free then attribute ]------------
	ToHide_EntyData.inst_id = -1;
	for (u_4b i = 0; i < ENTITY_LIMIT; ++i) {
		if (UnitVisualData[i].inst_id < inst) continue;
		UnitVisualData[i].inst_id -= 1;
	}
	//--------------[ Del in command ]--------------
	while (!IsFree.exchange(false, std::memory_order_acquire)) { SLP_THRD(1); };
	DRW_Commands[mesh].inst_count -= 1;
	for (i_4b i = mesh + 1; i < CmndCount; ++i) {
		DRW_Commands[i].inst_first -= 1;
	}
	//-------------[ Del in attribute ]-------------
	i_8b bytes_to_move = static_cast<i_8b>(InstCount - inst) << 5;
	if (bytes_to_move > 0) {
		std::memmove(&DRW_Attribs[inst], &DRW_Attribs[inst + 1], bytes_to_move);
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
#endif //MY_LIB_RENDER_REALISATION