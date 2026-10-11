#pragma once
#ifndef MY_LIB_OFFLINE_LOGICS_REALISATION
#define MY_LIB_OFFLINE_LOGICS_REALISATION
#define ToDraw(eIndex_to_draw) emg::Data.Logic.queue_add_to_draw_.psh(eIndex_to_draw)
#define CORRECT_STEP(position) position > WORLD_SIZE-2? 1: position

__forceinline void emg::logics::Offline_world_iteration() {
	lgc::classes::CWorld& world = *static_cast<lgc::classes::CWorld*>(WorldVaribles.wrl);
	for (u_2b offline_indx = 0; offline_indx < ENTITY_LIMIT; ++offline_indx) {
		if (emg::Data.Logic.unit_visual_data_[offline_indx].inst_id < 0) continue; //≈сли логика не обрабатываетс€ подробно
		mth::U2_2pos &unit_pos = emg::Data.Logic.unit_global_step_[offline_indx];
		


	}
#ifdef EMG_LOGIC_STDOUT
	_dbg_logic_procedures._dbg.logic_stdout();
#endif//EMG_LOGIC_STDOUT
}

#undef ToDraw
#undef CORRECT_STEP

#endif //MY_LIB_OFFLINE_LOGICS_REALISATION