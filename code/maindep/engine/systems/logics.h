#pragma once
#ifndef MY_LIB_LOGICS_REALISATION
#define MY_LIB_LOGICS_REALISATION

#define Enty_DRW emg::Data.Logic.unit_visual_data_ //Данные о перемещении оффлайн
#define Enty_POS emg::Data.Logic.unit_global_step_ //Данные о перемещении оффлайн
#define Enty_Curs emg::Data.Logic.online_manager_list_
#define Enty_List emg::Data.Logic.online_list_

#ifdef _DEBUG
	#ifdef EMG_LOGIC_STDOUT
	char char_map_world[WORLD_SIZE + 1][(WORLD_SIZE * 2) + 1];
	#define Poi_Drw(a,b) emg::Data.logic.essence_draw[emg::Data.logic.world_map[a][b]].mesh_id
	#endif//EMG_LOGIC_STDOUT
struct {
#ifdef DRAW_CMD_STDOUT
	void out_draw_cmd() {
		std::cout << "\nCMD_COUNT : " << emg_DRW.count_commands_ << std::endl;
		for (u_2b i = 0; i < emg_DRW.count_commands_; ++i) {
			std::cout << "    [" << i << "] :"
				<< "  [ inst_first : " << emg_CMD[i].inst_first << " ]; "
				<< "\t[ inst_count : " << emg_CMD[i].inst_count << " ]; "
				<< "\t[ vert_first : " << emg_CMD[i].vert_first << " ]; "
				<< "\t[ vert_count : " << emg_CMD[i].vert_count << " ]; "
				<< std::endl;
		}
	}
#endif
#ifdef DRAW_ATR_STDOUT
	void out_draw_atr() {
		std::cout << "\nINS_COUNT : " << emg::Data.Render.count_instaces_ << std::endl;
		for (u_4b i = 0; i < emg::Data.Render.count_instaces_; ++i) {
			std::cout << "    [" << i << "] :"
				<< "  yaw = " << emg_ATR[i].yaw << "; "
				<< "\tpich = " << emg_ATR[i].pich << "; "
				<< "\troll = " << emg_ATR[i].roll << "; "
				<< "\tx	= " << emg_ATR[i].x << "; "
				<< "\ty	= " << emg_ATR[i].y << "; "
				<< "\tz	= " << emg_ATR[i].z << "; "
				<< "\tAtlas_X = " << emg_ATR[i].Atlas_X << "; "
				<< "\tAtlas_Y = " << emg_ATR[i].Atlas_Y << "; "
				<< std::endl;
		}
	}
#endif
#ifdef DRAW_ENT_STDOUT
	void out_draw_ent() {
		std::cout << "\nENTITY_LIMIT : " << ENTITY_LIMIT << std::endl;
		for (u_4b i = 0; i < ENTITY_LIMIT; ++i) {
			std::cout << "    [" << i << "] : "
				<< "Mesh_id = " << Enty_DRW[i].mesh_id << "; "
				<< "Inst_id = " << Enty_DRW[i].inst_id << "; "
				<< std::endl;
		}
	}
#endif
#ifdef EMG_LOGIC_STDOUT
	void logic_stdout() {
		for (u_2b j, i = 0; i < WORLD_SIZE; ++i) {
			for (j = 0; j < WORLD_SIZE; ++j) {
				char_map_world[i][j * 2 + 1] = ' ';
				char_map_world[i][j * 2] =
					emg_MAP[i][j] == 0 ? ' ' :
					Poi_Drw(i, j) == 1 ? 'O' :
					Poi_Drw(i, j) == 2 ? 'S' :
					Poi_Drw(i, j) == 3 ? 'X' : '*';
			}
			char_map_world[i][WORLD_SIZE * 2] = '\n';
			char_map_world[i][WORLD_SIZE * 2 - 1] = '|';
			char_map_world[WORLD_SIZE][i * 2] = '-';
			char_map_world[WORLD_SIZE][i * 2 + 1] = '-';
		}
		char_map_world[WORLD_SIZE][WORLD_SIZE * 2] = '\0';
		system("cls");
		std::cout << (c_wrd)char_map_world;
	}
#endif//EMG_LOGIC_STDOUT
} _dbg_logic_procedures;
#endif //_DEBUG


namespace lgc {
	enum PosType : i_1b {
		NOTHING = '.',
		NOWALK = ' ',
		FOREST = 'Y',
		WATTER = 'F',
		VILAGE = 'O',
		CITY = '#',
		ROAD = '^'
	};
	class ITerrain {
	public:
		u_2b size_;
		virtual u_1b Typ(mth::U2_2pos& pos) = 0;
		virtual u_1b Typ(u_2b indx) = 0;
		virtual void Init(img::CStbImg& image, u_2b size) = 0;
		virtual void Print() = 0;
		virtual void Free() = 0;
	};
	class IWorld {
		#define SMngr mth::SI2b_ManagerList
		public:
			virtual void Print() = 0;
			virtual i_2b Del(mth::U2_2pos& pos) = 0;
			virtual i_2b Set(mth::U2_2pos& pos, img::CStbImg& _img_ptr) = 0;
			virtual SMngr* Entyes(mth::U2_2pos& pos) = 0;
			virtual t_sym  Region(mth::U2_2pos& pos) = 0;
		#undef SMngr
	};
	class IEntity {
		public:
			virtual i_2b Init(u_2b p, u_2b wrld_size) = 0;
			virtual void Print(lgc::IWorld* _wrld) = 0;
			virtual void Thnk(lgc::IWorld* _wrld) = 0;
			virtual void Draw(u_4b wrld_size) = 0;
	};
}

#include "logical/classes/CTerrain.h"
#include "logical/classes/CWorld.h"


struct {
	img::CStbImg img;
	lgc::IWorld* wrl;
	u_4b Win_W;
	u_4b Win_H;
	u_4b Image_size;
	u_2b Population;
	void Init() {
		wrl = new lgc::classes::CWorld();
	}
	void Free() {
		delete static_cast<lgc::classes::CWorld*>(wrl);
	}
} WorldVaribles;

__forceinline void emg::logics::Init() {
	WorldVaribles.Init();
	Enty_POS[0].x = 0; //положительные = влево
	Enty_POS[0].y = 0; //положительные = дальш
	Enty_DRW[0].mesh_id = 0;
	Enty_DRW[0].txtr_id = 1;
	Enty_DRW[0].anim_id = -1;
	Enty_DRW[0].inst_id = -1;
	for (u_2b x{}, y{}, i = 1; i < ENTITY_LIMIT; ++i) {
		Enty_POS[i].x = std::rand() & 32; //положительные = влево
		Enty_POS[i].y = std::rand() & 32; //положительные = дальш

		Enty_DRW[i].mesh_id = 1 + i % 3;
		Enty_DRW[i].txtr_id = 1;
		Enty_DRW[i].anim_id = -1;
		Enty_DRW[i].inst_id = -1;
		Enty_DRW[i].ID_Material = 0;
		Enty_DRW[i].ID_Instance = 0;

	}
	Enty_Curs.Attach(&Enty_List);


	for (i_4b i = 2; i < 8; ++i) {
		emg::Data.Logic.queue_add_to_draw_.Push(i);
	}
}
__forceinline void emg::logics::Exit() {
	WorldVaribles.Free();
	Enty_Curs.deleteList();
	Enty_Curs.Release();
}

#undef Enty_DRW
#undef Enty_POS
#undef Enty_STP
#undef Enty_Curs
#undef Enty_List
#ifdef EMG_LOGIC_STDOUT
#undef EMG_LOGIC_STDOUT
#undef Poi_Drw
#endif

#include "logical/offline.h"
#include "logical/online.h"

#endif //MY_LIB_LOGICS_REALISATION