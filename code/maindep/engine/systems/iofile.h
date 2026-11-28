#pragma once
#ifndef MY_LIB_IOFILES_REALISATION
#define MY_LIB_IOFILES_REALISATION
//===========================================================================================
#define FIRST_SYM(word, a, b, c) (word)[i] == (a) && (word)[i+1] == (b) && (word)[i+2] == (c)
#define LAST_SYM(word) (word)[i+3] == ':' && (word)[i+4] == '=' && (word)[i+5] == ' '
#define SYM_MeshCompShader(word) FIRST_SYM( (word), 'c', 'm', 'p') && LAST_SYM( (word) )
#define SYM_MeshFragShader(word) FIRST_SYM( (word), 'f', 'r', 'g') && LAST_SYM( (word) )
#define SYM_MeshVertShader(word) FIRST_SYM( (word), 'v', 'r', 't') && LAST_SYM( (word) )
#define SYM_MeshMeshData(word) FIRST_SYM( (word), 'm', 's', 'h') && LAST_SYM( (word) )
#define SYM_RegionVertShader(word) FIRST_SYM( (word), 'r', 'v', 't') && LAST_SYM( (word) )
#define SYM_RegionFragShader(word) FIRST_SYM( (word), 'r', 'f', 'g') && LAST_SYM( (word) )
#define SYM_RegionMeshData(word) FIRST_SYM( (word), 'm', 'a', 'p') && LAST_SYM( (word) )
#define SYM_TexturesAtlas(word) FIRST_SYM( (word), 'a', 't', 'l') && LAST_SYM( (word) )
//===========================================================================================
#define WriteFileName(ThisDataName)		{				\
		t_wrd& fileName = (ThisDataName);				\
		fileName = new t_sym[64];						\
		for (j = 0, i += 6; i < fileSize; ++j, ++i) {	\
			fileName[j] = GamedataFileText[i];			\
			if (GamedataFileText[j + i] == '*') {		\
				fileName[j] = '\0';						\
				break;									\
			}											\
		}}

emg::gamedata::GameDataMap emg::gamedata::Load() {
	emg::gamedata::GameDataMap FileMap{};
	t_wrd GamedataFileText;
	u_8b fileSize;
	GamedataFileText = iof::load_file_txt(emg::Data.Files.files_data_map_, &fileSize);
	if (!GamedataFileText || fileSize < 128) {
		Logger.SetError("ERROR::[ iofile.h ]: GameDtata load [");
		Logger.SetError(emg::Data.Files.files_data_map_);
		Logger.SetError(" ] \n");
		return FileMap;
	}
	for (i_4b j, i = 0; i < fileSize - 5; ++i) {
		if (SYM_MeshMeshData(GamedataFileText)) {
			WriteFileName(FileMap.models_mesh_array_[FileMap.models_mesh_count_]);
			FileMap.models_mesh_count_ += 1;
		}
		if (SYM_RegionMeshData(GamedataFileText)) {
			WriteFileName(FileMap.terrain_base_name_);
		}
		if (SYM_TexturesAtlas(GamedataFileText)) {
			WriteFileName(FileMap.texture_atlas_name_);
		}
		if (SYM_MeshCompShader(GamedataFileText))   WriteFileName(FileMap.models_cmp_shader_names_);
		if (SYM_MeshVertShader(GamedataFileText))   WriteFileName(FileMap.models_vrt_shader_names_);
		if (SYM_MeshFragShader(GamedataFileText))   WriteFileName(FileMap.models_frg_shader_names_);
		if (SYM_RegionVertShader(GamedataFileText)) WriteFileName(FileMap.models_vrt_shader_names_);
		if (SYM_RegionFragShader(GamedataFileText)) WriteFileName(FileMap.models_frg_shader_names_);
	}
}
//===========================================================================================
#endif//MY_LIB_IOFILES_REALISATION