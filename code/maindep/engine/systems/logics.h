#pragma once
#ifndef MY_LIB_LOGICS_REALISATION
#define MY_LIB_LOGICS_REALISATION

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
}

#include "logical/classes/CTerrain.h"
#include "logical/classes/CWorld.h"

#define Enty_DRW emg::Data.Logic.unit_visual_data_
#define Enty_POS emg::Data.Logic.unit_global_step_
#define Enty_Curs emg::Data.Logic.online_manager_list_
#define Enty_List emg::Data.Logic.online_list_

struct {
	img::CStbImg img;
	lgc::IWorld* wrl{0};
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
	for (u_2b x{}, y{}, i = 0; i < ENTITY_LIMIT; ++i) {
		Enty_POS[i].x = std::rand() % 64; //положительные = влево
		Enty_POS[i].y = std::rand() % 64; //положительные = дальшe
		Enty_DRW[i].mesh_id = 1 + i % 3;
		Enty_DRW[i].txtr_id = 1;
		Enty_DRW[i].anim_id = -1;
		Enty_DRW[i].inst_id = -1;
	}
	Enty_Curs.Attach(&Enty_List);
	for (i_4b i = 0; i < 8; ++i) {
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
#undef Enty_Curs
#undef Enty_List

#ifdef EMG_LOGIC_STDOUT
#undef EMG_LOGIC_STDOUT
#endif

#include "logical/offline.h"
#include "logical/online.h"

#endif //MY_LIB_LOGICS_REALISATION