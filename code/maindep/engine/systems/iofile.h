#pragma once
#ifndef MY_LIB_IOFILES_REALISATION
#define MY_LIB_IOFILES_REALISATION

#define FTYPES emg::iofile::FILES_TYPE

#define Sym_CMP(Symbols) Symbols[i] == 'c' && Symbols[i+1] == 'm' && Symbols[i+2] == 'p' && Symbols[i+3] == ':' && Symbols[i+4] == '=' && Symbols[i+5] == ' '
#define Sym_FRG(Symbols) Symbols[i] == 'f' && Symbols[i+1] == 'r' && Symbols[i+2] == 'g' && Symbols[i+3] == ':' && Symbols[i+4] == '=' && Symbols[i+5] == ' '
#define Sym_VRT(Symbols) Symbols[i] == 'v' && Symbols[i+1] == 'r' && Symbols[i+2] == 't' && Symbols[i+3] == ':' && Symbols[i+4] == '=' && Symbols[i+5] == ' '
#define Sym_MSH(Symbols) Symbols[i] == 'm' && Symbols[i+1] == 's' && Symbols[i+2] == 'h' && Symbols[i+3] == ':' && Symbols[i+4] == '=' && Symbols[i+5] == ' '
#define Sym_ATL(Symbols) Symbols[i] == 'a' && Symbols[i+1] == 't' && Symbols[i+2] == 'l' && Symbols[i+3] == ':' && Symbols[i+4] == '=' && Symbols[i+5] == ' '

// **********************************************************************************************
__forceinline Bool IsCompShader(c_wrd t, i_4b i) { return Sym_CMP(t); }
__forceinline Bool IsVertShader(c_wrd t, i_4b i) { return Sym_VRT(t); }
__forceinline Bool IsFragShader(c_wrd t, i_4b i) { return Sym_FRG(t); }
__forceinline emg::iofile::ShaderLoadedData* get_saheder_text(Bool(*foo)(c_wrd, i_4b) ) {
	emg::iofile::ShaderLoadedData* shd{ nullptr };
	t_wrd DataFileText{ nullptr };
	t_wrd shaderName{ nullptr };
	u_8b Shader_file_size{ 0 };
	u_8b Data_file_size{ 0 };
	DataFileText = iof::load_file_txt(emg::Data.Files.files_data_map_, &Data_file_size);
	if (!DataFileText) {
		Logger.SetError("ERROR::[ iofile.h ]: get_saheder_text(): [");
		Logger.SetError(emg::Data.Files.files_data_map_);
		Logger.SetError(" ] \n");
		return nullptr;
	}
	shd = static_cast<emg::iofile::ShaderLoadedData*>(malloc(sizeof(emg::iofile::ShaderLoadedData)));
	if (!shd) {
		Logger.SetError("ERROR::[ iofile.h ]: get_saheder_text(): ALLOC SHADER STRUCT\n");
		return nullptr;
	}
	shd->text = nullptr;
	for (u_4b j, i = 0; i < Data_file_size - 5; ++i) {
		if (!foo(DataFileText, i)) continue;
		shaderName = static_cast<t_wrd>(malloc(64));
		if (!shaderName) break;
		for (j = 0, i += 6; i < Data_file_size; ++i, ++j) {
			shaderName[j] = DataFileText[i];
			if (shaderName[j] == '*') break;
		} shaderName[j] = '\0';
		shd->text = iof::load_file_txt(shaderName, &Shader_file_size);
	}
	free(DataFileText);
	if (!shaderName) {
		Logger.SetError("ERROR::[ iofile.h ]: get_saheder_text(): LOST SHADER NAME\n");
		return nullptr;
	} 
	free(shaderName);
	if (!shd->text) {
		Logger.SetError("ERROR::[ iofile.h ]: get_saheder_text(): LOST SHADER TEXT\n");
		return nullptr;
	}
	return shd;
}

// **********************************************************************************************



__forceinline PMem emg::iofile::Load(FTYPES fType) {
	switch (fType){
		case FTYPES::COMP_SHADER: return static_cast<PMem>(get_saheder_text(IsCompShader));
		case FTYPES::FRAG_SHADER: return static_cast<PMem>(get_saheder_text(IsFragShader));
		case FTYPES::VERT_SHADER: return static_cast<PMem>(get_saheder_text(IsVertShader));
		default: break;
	}
	return nullptr;
}




#undef Sym_CMP
#undef Sym_FRG
#undef Sym_VRT
#undef Sym_MSH
#undef Sym_ATL

#endif//MY_LIB_IOFILES_REALISATION