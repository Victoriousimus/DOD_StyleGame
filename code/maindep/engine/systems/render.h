#pragma once
#ifndef MY_LIB_RENDER_REALISATION
#define MY_LIB_RENDER_REALISATION
//===========================================================================================
#define CHECK_SYMBOLS_5pos_cmp(Symbols) Symbols[i] == 'c' && Symbols[i+1] == 'm' && Symbols[i+2] == 'p' && Symbols[i+3] == ':' && Symbols[i+4] == '=' && Symbols[i+5] == ' '
#define CHECK_SYMBOLS_5pos_frg(Symbols) Symbols[i] == 'f' && Symbols[i+1] == 'r' && Symbols[i+2] == 'g' && Symbols[i+3] == ':' && Symbols[i+4] == '=' && Symbols[i+5] == ' '
#define CHECK_SYMBOLS_5pos_vrt(Symbols) Symbols[i] == 'v' && Symbols[i+1] == 'r' && Symbols[i+2] == 't' && Symbols[i+3] == ':' && Symbols[i+4] == '=' && Symbols[i+5] == ' '
#define CHECK_SYMBOLS_5pos_msh(Symbols) Symbols[i] == 'm' && Symbols[i+1] == 's' && Symbols[i+2] == 'h' && Symbols[i+3] == ':' && Symbols[i+4] == '=' && Symbols[i+5] == ' '
#define CHECK_SYMBOLS_5pos_atl(Symbols) Symbols[i] == 'a' && Symbols[i+1] == 't' && Symbols[i+2] == 'l' && Symbols[i+3] == ':' && Symbols[i+4] == '=' && Symbols[i+5] == ' '

#define emg_PAR emg::Data.Window.Params
#define emg_INP emg::Data.Window.Input
#define emg_CMD emg::Data.Render.DrawCommands_
#define emg_ATR emg::Data.Render.InstnAttribs_
#define emg_VEL emg::Data.Render.InstnOffSets_
#define emg_EPT emg::Data.Render.InstnEndPoints_
#define emg_Render emg::Data.Render
#define Buffer emg::Data.Render.GPU
#define Camera emg::Data.Render.Camera
#define TEXTURES_LOAD_MODE 3

// Обновление массива атрибутов
__forceinline void Interpolation_Draw_Attributes() {
}

// Считать ввод для камеры, изменить позицию и углы
__forceinline void scene_update_camera() {
	Camera.angle_.x += static_cast<f_4b>(emg_INP.cursor_displacement_.x) * emg_PAR.Input.mouse_sensitivity_;
	Camera.angle_.y += static_cast<f_4b>(emg_INP.cursor_displacement_.y) * emg_PAR.Input.mouse_sensitivity_;
	//Нормализуем поворот по осям
	if (Camera.angle_.x < 0) Camera.angle_.x += MTH_RADIANS;
	else if (Camera.angle_.x > MTH_RADIANS) Camera.angle_.x -= MTH_RADIANS;
	if (Camera.angle_.y > MTH_MAX_ANG_Y) Camera.angle_.y = MTH_MAX_ANG_Y;
	else if (Camera.angle_.y < MTH_MIN_ANG_Y) Camera.angle_.y = MTH_MIN_ANG_Y;
	//Запицем для оптимизации вызова cos(x), sin(x)
	Camera.ang_front_.x = sinf(Camera.angle_.x);
	Camera.ang_front_.z = cosf(Camera.angle_.x);
	//Считываем смещение по осям
	if (emg_INP.key_flags_[EMG_INPUT_FRONT]) {
		Camera.postn_.x += Camera.velocity_ * Camera.ang_front_.x;
		Camera.postn_.z += Camera.velocity_ * Camera.ang_front_.z;
	}
	if (emg_INP.key_flags_[EMG_INPUT_BACK]) {
		Camera.postn_.x -= Camera.velocity_ * Camera.ang_front_.x;
		Camera.postn_.z -= Camera.velocity_ * Camera.ang_front_.z;
	}
	if (emg_INP.key_flags_[EMG_INPUT_LEFT]) {
		Camera.postn_.x += Camera.velocity_ * Camera.ang_front_.z;
		Camera.postn_.z -= Camera.velocity_ * Camera.ang_front_.x;
	}
	if (emg_INP.key_flags_[EMG_INPUT_RIGTH]) {
		Camera.postn_.x -= Camera.velocity_ * Camera.ang_front_.z;
		Camera.postn_.z += Camera.velocity_ * Camera.ang_front_.x;
	}
	
	// Вверх / Вниз
	if (emg_INP.key_flags_[EMG_INPUT_UP])   Camera.postn_.y += Camera.velocity_; //Z
	if (emg_INP.key_flags_[EMG_INPUT_DOWN]) Camera.postn_.y -= Camera.velocity_; //X

	//Обновим матрицу вида
	Camera.ang_front_.y = cosf(Camera.angle_.y);
	Camera.ang_front_.x *= Camera.ang_front_.y;
	Camera.ang_front_.z *= Camera.ang_front_.y;
	Camera.ang_front_.y = sinf(Camera.angle_.y);
	Camera.ang_front_ = glm::normalize(Camera.ang_front_);
	Camera.cam_view_matrix_ = Camera.projection_matrix_ *
		glm::lookAt( Camera.postn_, Camera.postn_ + Camera.ang_front_, Camera.null_view_ );
}

//Загрузка шейдерных программ
__forceinline void load_saheder_progs(u_4b* comp_Shader, u_4b* vert_Shader, u_4b* frag_Shader, i_4b* shader_success) {
	u_4b& compShader = *comp_Shader;
	u_4b& vertShader = *vert_Shader;
	u_4b& fragShader = *frag_Shader;
	i_4b& success = *shader_success;
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
		u_4b* shader_ref{ nullptr };
		u_4b shader_type = 0;
		//Проверяем тэги в поисках интересующего символа
		if (CHECK_SYMBOLS_5pos_cmp(fileText)) {
			shader_type = GL_COMPUTE_SHADER;
			shader_ref = &compShader;
		}
		else
		if (CHECK_SYMBOLS_5pos_frg(fileText)) {
			shader_type = GL_FRAGMENT_SHADER;
			shader_ref = &fragShader;
		}
		else
		if (CHECK_SYMBOLS_5pos_vrt(fileText)) {
			shader_type = GL_VERTEX_SHADER;
			shader_ref = &vertShader;
		}
		else continue;
		//если символ шейдера найден, записываем его название
		sahderName = static_cast<t_wrd>(malloc(64));
		if (!sahderName) {
			if (shader_type == GL_COMPUTE_SHADER)
				Logger.SetError("ERROR::[ render.h ]: load_prog_to_GPU(): cmp:= : sahderName = malloc(64)\n");
			if (shader_type == GL_FRAGMENT_SHADER)
				Logger.SetError("ERROR::[ render.h ]: load_prog_to_GPU(): frg:= : sahderName = malloc(64)\n");
			if (shader_type == GL_VERTEX_SHADER)
				Logger.SetError("ERROR::[ render.h ]: load_prog_to_GPU(): vrt:= : sahderName = malloc(64)\n");
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
			if (compShader) glDeleteShader(compShader);
			if (vertShader) glDeleteShader(vertShader);
			if (fragShader) glDeleteShader(fragShader);
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
// Загрузка массивов аттрибутов вершин
__forceinline void load_this_moDels(f_4b** result_array, u_8b* attrib_count, i_4b* moDels_count) {
	f_4b* all_VertexAttribsArray;
	f_4b** vecArr = nullptr;	// указатель на массивы вершин
	u_8b* vecLen = nullptr;		// указатели на длинны массивов
	u_8b lengt_accum = 0;
	i_4b moDel_count = 0;
	t_wrd fileNames[MODELS_LIMIT] = {0};
	t_wrd fileText;

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
		if(CHECK_SYMBOLS_5pos_msh(fileText)) {
			fileNames[moDel_count] = new t_sym[64];
			if (!fileNames[moDel_count]) {
				Logger.SetError("ERROR::[ render.h ]: load_this_moDels(result_array, attrib_count): fileNames[");
				Logger.SetError(moDel_count);
				Logger.SetError("] MALLOC ERROR\n");
				for (j = 0; j < moDel_count; ++j) { delete[] fileNames[j]; }
				free(fileText);
				*result_array = nullptr;
				*moDels_count = 0;
				*attrib_count = 0;
				return;
			}
			for (j = 0, i += 6; i < lengt_accum; ++i, ++j) {
				fileNames[moDel_count][j] = fileText[i];
				if (fileNames[moDel_count][j] == '*') break;
			}
			fileNames[moDel_count][j] = '\0';
			++moDel_count;
		}
	} 
	free(fileText);
	// Инициализируем временное хранилище памяти для штучных мешей, для дальнейшего объеденения
	vecArr = new f_4b*[moDel_count];	// <- Массив указателей на масссивы вершин
	vecLen = new u_8b[moDel_count];	// <- Массив длин массивов по указателю
	if (!vecArr || !vecLen) {
		Logger.SetError("ERROR::[ render.h ]:vecArr = malloc(moDel_count * size); vecLen = malloc(moDel_count * size)\n");
		for (i_4b i = 0; i < moDel_count; ++i) { delete[] fileNames[i]; }
		if(vecLen) delete[] vecLen;
		if(vecArr) delete[] vecArr;
		*result_array = nullptr;
		*moDels_count = 0;
		*attrib_count = 0;
		return;
	}
	lengt_accum = 0;
	for (i_4b i = 0; i < moDel_count; ++i) { vecArr[i] = nullptr; }
	// Загружаем во вершины в массивы и формируем под него DrawComand
#ifdef LOAD_CMD_STDOUT
	std::cout << "--------------------------------------------------\n" << std::endl;
#endif// LOAD_CMD_STDOUT
	for (i_4b i = 0; i < moDel_count; ++i) {
		vecArr[i] = static_cast<f_4b*>(iof::load_file_ultrafast(fileNames[i], &vecLen[i]));// vecLen[i] - будет записана длинна массива в байтах
		delete[] fileNames[i];
		if (!vecArr[i]) {
			Logger.SetError("ERROR::[ render.h ]:vecArr[i] = static_cast<f_4b*>(iof::load_file_ultrafast(fileNames[i], &vertx_count));\n");
			// Подчищаем временные хранилища мешей
			for (i_4b j = i; j > -1; --j) { iof::free_ultrafast_loaded_data(vecArr[j]); }
			for (i_4b j = 0; j < moDel_count; ++j) { delete[] fileNames[j]; }
			delete[] vecLen;
			delete[] vecArr;
			*result_array = nullptr; 
			*moDels_count = 0;
			*attrib_count = 0;
			return;
		}
		emg_CMD[i].inst_count = 0; // Количество инстансов
		emg_CMD[i].vert_count = static_cast<u_4b>(vecLen[i]) >> 5;	// Количество вершин для примитива
		emg_CMD[i].inst_first = i < 1 ? 0 : // Базовый инстанс (добавляется к gl_instance_index)
			emg_CMD[i - 1].inst_first +
			emg_CMD[i - 1].inst_count;
		emg_CMD[i].vert_first = i < 1 ? 0 :	// Начальный индекс в вершинном массиве
			emg_CMD[i - 1].vert_first +
			emg_CMD[i - 1].vert_count;
		vecLen[i] = static_cast<u_4b>(vecLen[i] >> 2);			// Сдвиг чтобы сичтать f_4b
		lengt_accum += vecLen[i];
#ifdef LOAD_CMD_STDOUT
		std::cout
			<< "------------------------------------\n"
			<< "[ inst_first : " << emg_CMD[i].inst_first << " ]; "
			<< "[ inst_count : " << emg_CMD[i].inst_count << " ]; "
			<< "[ vert_first : " << emg_CMD[i].vert_first << " ]; "
			<< "[ vert_count : " << emg_CMD[i].vert_count << " ]; "
			<< "------------------------------------\n"
			<< std::endl;
#endif// LOAD_CMD_STDOUT
	}
#ifdef LOAD_CMD_STDOUT
	std::cout<< "--------------------------------------------------\n" << std::endl;
#endif// LOAD_CMD_STDOUT
	all_VertexAttribsArray = static_cast<f_4b*>(malloc(lengt_accum * sizeof(f_4b)));
	if (!all_VertexAttribsArray) {
		Logger.SetError("ERROR::[ render.h ]: static_cast<f_4b*>(malloc(lengt_accum * sizeof(f_4b)))\n");
		for (i_4b i = 0; i < moDel_count;++i) { iof::free_ultrafast_loaded_data(vecArr[i]); }
		delete[] vecLen;
		delete[] vecArr;
		*result_array = nullptr;
		*moDels_count = 0;
		*attrib_count = 0;
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
	*moDels_count = moDel_count;
	*attrib_count = lengt_accum;
	*result_array = all_VertexAttribsArray;
}

// Init() < - Настройка параметров отрисовки
__forceinline void init_draw_params() {
	if (!gladLoadGL()) {
		Logger.SetError("ERROR::[ render.h ]:Failed to initialize GLAD\n");
		return;
	}
	glViewport(0, 0, emg_PAR.Size.width_, emg_PAR.Size.heigth_);
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

	Camera.angle_.x = 5.94f;
	Camera.angle_.y = -0.7f;

	Camera.postn_.x = 11.66f;
	Camera.postn_.y = 18.29f;
	Camera.postn_.z = -26.1f;

	Camera.projection_matrix_ = glm::perspective(
		glm::radians(Camera.fov_),
		emg::Data.Window.DetailData.scale_koef_,
		Camera.near_,
		Camera.far_
	);
	Camera.null_view_ = v3_12b(0.0f, 1.0f, 0.0f);
	Camera.cam_view_matrix_ = m4_64b(1) * Camera.projection_matrix_;

	emg_Render.ViewMatrix_size_ = sizeof(m4_64b);
	emg_Render.ViewMatrix_refc_ = glm::value_ptr(Camera.cam_view_matrix_);
	
	for (u_4b i = 0; i < ONLINE_LIMIT; ++i) {
		emg_ATR[i].pich = 0; //Тангаж
		emg_ATR[i].yaw  = 0; //Рысканье
		emg_ATR[i].roll = 0; //Крен
		emg_ATR[i].x = 0; //положительные = влево
		emg_ATR[i].y = 0; //положительные = дальш
		emg_ATR[i].z = 0; //положительные = выше
	}
}

// Init() < - Загрузка шейдера расчета и отрисовки в GPU
__forceinline void load_prog_to_GPU() {
	u_4b compShader = 0;
	u_4b vertShader = 0;
	u_4b fragShader = 0;
	i_4b success = 0;

	load_saheder_progs(&compShader, &vertShader, &fragShader, &success);

	// Создание шейдерной программы и линковка расчетного шейдера
	Buffer.index_compute_shader_ = glCreateProgram();
	glAttachShader(Buffer.index_compute_shader_, compShader);
	glLinkProgram(Buffer.index_compute_shader_);
	// Проверка ошибок линковки расчетного шейдера
	glGetProgramiv(Buffer.index_compute_shader_, GL_LINK_STATUS, &success);
	if (!success) {
		glDeleteShader(compShader);
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		char infoLog[512];
		glGetProgramInfoLog(Buffer.index_compute_shader_, 512, NULL, infoLog);
		Logger.SetError("ERROR::SHADER::PROGRAM::LINKING_FAILED [COMPUTE SHADER]  LOG:\n");
		Logger.SetError(infoLog);
		Logger.SetError("\n");
		return;
	}
	// Создание шейдерной программы и линковка шейдеров
	Buffer.index_drawind_shader_ = glCreateProgram();
	glAttachShader(Buffer.index_drawind_shader_, vertShader);
	glAttachShader(Buffer.index_drawind_shader_, fragShader);
	glLinkProgram(Buffer.index_drawind_shader_);
	// Проверка ошибок линковки
	glGetProgramiv(Buffer.index_drawind_shader_, GL_LINK_STATUS, &success);
	if (!success) {
		glDeleteShader(compShader);
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		char infoLog[1024];
		glGetProgramInfoLog(Buffer.index_drawind_shader_, 1024, NULL, infoLog);
		Logger.SetError("ERROR::SHADER::PROGRAM::LINKING_FAILED [ BASE_DRAW_SHADER ]  LOG:\n");
		Logger.SetError(infoLog);
		Logger.SetError("\n");
		return;
	}
	// Удаление шейдеров (они уже в программе)
	glDeleteShader(compShader);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

// Init() < - Загрузка атрибутов отрисовки в GPU
__forceinline void load_attr_to_GPU() {
	m4_64b InstancesMatrix[ONLINE_LIMIT] = {0};
	// Cоздаем буфферы
	glGenBuffers(1, &Buffer.index_UBO_);
	glGenBuffers(1, &Buffer.index_matrixs_inderect_);
	glGenBuffers(1, &Buffer.index_attribs_inderect_);
	// Привязываем буффер
	glBindBuffer(GL_UNIFORM_BUFFER, Buffer.index_UBO_);
	glBufferData(GL_UNIFORM_BUFFER, emg_Render.ViewMatrix_size_, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, emg_Render.ViewMatrix_size_, emg_Render.ViewMatrix_refc_);
	// Привязка к слоту 0
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, Buffer.index_UBO_);
	// Передадим смещения для моделей, которые будут обновляться каждый кадр
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Buffer.index_attribs_inderect_);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(emg_ATR), emg_ATR, GL_DYNAMIC_DRAW);
	// Привязка к слоту 1
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, Buffer.index_attribs_inderect_);
	// Передадим матрицы трансформации, которые будут расчитаны в шейдере
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Buffer.index_matrixs_inderect_);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(InstancesMatrix), InstancesMatrix, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT);
	// Привязка к слоту 8
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, Buffer.index_matrixs_inderect_);
	// Отвязываем буффер
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);
}
// Init() < - Заполняет загружает и передает модели в матрицу
__forceinline void load_mesh_to_GPU() {
	f_4b* all_VertexAttribsArray;
	u_8b Vert_Сount;
	// vertx_count - будет записанo количество элемнтов массива
	load_this_moDels(&all_VertexAttribsArray, &Vert_Сount, &emg_Render.count_commands_);
	if (!Vert_Сount) return;
	// Создание VAO
	glGenVertexArrays(1, &Buffer.index_VAO_);
	glBindVertexArray(Buffer.index_VAO_);
	// Привязка и заполнение Vertex Buffer Object
	glGenBuffers(1, &Buffer.index_VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, Buffer.index_VBO_);
	//glBufferStorage( ***, ***, 0) - передав 0 скажем драйверу,что данные не изменяются
	glBufferStorage(GL_ARRAY_BUFFER, Vert_Сount * sizeof(f_4b), all_VertexAttribsArray, 0);
	// Координаты (layout location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f_4b), (PMem)0);
	glEnableVertexAttribArray(0);
	// Нормали (layout location = 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f_4b), (PMem)(3 * sizeof(f_4b)));
	glEnableVertexAttribArray(1);
	// Цвета (layout location = 2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f_4b), (PMem)(6 * sizeof(f_4b)));
	glEnableVertexAttribArray(2);
	// Массив команд отрисовки
	glGenBuffers(1, &Buffer.index_comands_inderect_);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, Buffer.index_comands_inderect_);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(emg_CMD), emg_CMD, GL_STATIC_DRAW);
	// Деактивация буферов
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
	// Отчистка памяти массива мешей
	free(all_VertexAttribsArray);
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
		if (CHECK_SYMBOLS_5pos_atl(fileText)) {
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
	glGenTextures(emg_Render.count_texutres_, Buffer.textures_index_);
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
		glBindTexture(GL_TEXTURE_2D, Buffer.textures_index_[i]);
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
	init_draw_params(); 
	if (Logger.error_flag_ < 0) return;
	init_view_matrix();
	if (Logger.error_flag_ < 0) return;
	load_prog_to_GPU(); 
	if (Logger.error_flag_ < 0) return;
	load_attr_to_GPU(); 
	if (Logger.error_flag_ < 0) return;
	load_txtr_to_GPU();
	if (Logger.error_flag_ < 0) return;
	load_mesh_to_GPU(); 
	if (Logger.error_flag_ < 0) return;
}
// Oчистка ресурсов
__forceinline void emg::render::Exit() {
	// Перед очисткой ресурсов, принудительно дождаться завершения всех операций GPU
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glUseProgram(NULL);
	glFinish();
	glDeleteTextures(emg_Render.count_texutres_, Buffer.textures_index_);
	glDeleteVertexArrays(1, &Buffer.index_VAO_);
	glDeleteBuffers(1, &Buffer.index_VBO_);
	glDeleteBuffers(1, &Buffer.index_UBO_);
	glDeleteBuffers(1, &Buffer.index_comands_inderect_);
	glDeleteBuffers(1, &Buffer.index_matrixs_inderect_);
	glDeleteBuffers(1, &Buffer.index_attribs_inderect_);
	glDeleteProgram(Buffer.index_drawind_shader_);
	glDeleteProgram(Buffer.index_compute_shader_);
}
// Метод отрисовки сцены
__forceinline void emg::render::CreateFrame() {
	// Очистка буфера цвета
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Включаем шейдер вычислений
	while (!emg_Render.draw_command_is_free_.exchange(false, std::memory_order_acquire)) {};
	Interpolation_Draw_Attributes();
	glUseProgram(Buffer.index_compute_shader_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Buffer.index_attribs_inderect_);
	{
		i_4b inst_stride = emg_Render.count_instaces_ << 5;		// =  X * 32
		if (inst_stride > 0) {
			glBufferData(GL_SHADER_STORAGE_BUFFER, inst_stride, emg_ATR, GL_DYNAMIC_DRAW);
		}
	}
	// Привязка к слоту 1
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, Buffer.index_attribs_inderect_);
	//Запуск расчетного шейдера
	glDispatchCompute(emg_Render.count_work_group_, 1, 1);
	//Обновить позицию камеры (считать ввод для камеры)
	scene_update_camera();
	//Проверка окончания работы расчетного шейдера
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//После рсчета переходим к рендерингу
	glUseProgram(Buffer.index_drawind_shader_);
	//Передаем матрицу камеры
	glBindBuffer(GL_UNIFORM_BUFFER, Buffer.index_UBO_);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, emg_Render.ViewMatrix_size_, emg_Render.ViewMatrix_refc_);
	//Активация вершинного массива и массива команд
	glBindVertexArray(Buffer.index_VAO_);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, Buffer.index_comands_inderect_);
	//Отслеживаем изменения в буфере команд отрисовкиs
	if (emg_Render.draw_command_update_.load(std::memory_order_acquire)) {
		i_4b cmd_stride = emg_Render.count_commands_ << 4;	// =  X * 16
		if (cmd_stride > 0) {
			glBufferData(GL_DRAW_INDIRECT_BUFFER, cmd_stride, emg_CMD, GL_STATIC_DRAW);
			emg_Render.draw_command_update_.store(false, std::memory_order_release);
		}
	}
	emg_Render.draw_command_is_free_.store(true, std::memory_order_release);
	//Активация текстур
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Buffer.textures_index_[1]);
	//Отрисовка всех инстансов ( режим отрисовки, начало, количество команд, шаг )
	glMultiDrawArraysIndirect(GL_TRIANGLES,0, emg_Render.count_commands_,sizeof(mth::SDrawCommand));
	//Деактивация вершинного массива и буфферов
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, NULL);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, NULL);
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);
	glBindVertexArray(NULL);
	//----------------------------------------------
}

#undef emg_PAR
#undef emg_INP
#undef emg_CMD
#undef emg_ATR
#undef emg_VEL
#undef emg_EPT
#undef Buffer
#undef Camera
#undef emg_Render
#undef CHECK_SYMBOLS_5pos_cmp
#undef CHECK_SYMBOLS_5pos_frg
#undef CHECK_SYMBOLS_5pos_vrt
#undef CHECK_SYMBOLS_5pos_msh
#undef CHECK_SYMBOLS_5pos_atl
#undef TEXTURES_LOAD_MODE
//===========================================================================================
#define UnitVisualData emg::Data.Logic.unit_visual_data_
#define DRW_Commands emg::Data.Render.DrawCommands_
#define DRW_Attribs emg::Data.Render.InstnAttribs_
//Добавить сущность в рендер : emg::Data.Render.essence_add_to_show_id_
__forceinline void emg::render::InserInRenderPipline() {
	mth::SEssenceMeta& ToShow_EntyData = UnitVisualData[emg::Data.Render.essence_add_to_show_id_];
	std::atomic<Bool>& IsFree = emg::Data.Render.draw_command_is_free_;
	std::atomic<Bool>& Update = emg::Data.Render.draw_command_update_;
	i_4b& InstCount = emg::Data.Render.count_instaces_;
	i_4b& CmndCount = emg::Data.Render.count_commands_;
	i_4b& WorkCount = emg::Data.Render.count_work_group_;

	if (InstCount >= ONLINE_LIMIT - 1) {
		Logger.SetError("ERROR::[ logic.h ]: add_active_essence_to_render()\n");
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
	size_t bytes_to_move = (InstCount - inst);
	if (bytes_to_move > 0) { 
		std::memmove(&DRW_Attribs[inst + 1], &DRW_Attribs[inst], bytes_to_move << 5); 
	}
	DRW_Attribs[inst].ID_Instance = ToShow_EntyData.inst_id;
	DRW_Attribs[inst].ID_Material = ToShow_EntyData.txtr_id;
	DRW_Attribs[inst].x = 0;
	DRW_Attribs[inst].y = 0;
	InstCount += 1;
	WorkCount = (InstCount + 127) >> 7;
	//----------------------------------------------
	Update.store(true, std::memory_order_release);
	IsFree.store(true, std::memory_order_release);
}
//Удалить сущность из рендера : emg::Data.Render.essence_add_to_hide_id_
__forceinline void emg::render::EraseInRenderPipline() {
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
	InstCount -= 1;
	size_t bytes_to_move = (InstCount - inst);
	std::memcpy(&DRW_Attribs[inst], &DRW_Attribs[inst+1], bytes_to_move << 6);
	WorkCount = (InstCount + 127) >> 7;
	//----------------------------------------------
	Update.store(true, std::memory_order_release);
	IsFree.store(true, std::memory_order_release);
}
#undef UnitVisualData 
#undef DRW_Commands
#undef DRW_Attribs
//===========================================================================================
#endif //MY_LIB_RENDER_REALISATION