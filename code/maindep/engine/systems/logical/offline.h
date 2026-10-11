#pragma once
#ifndef MY_LIB_OFFLINE_LOGICS_REALISATION
#define MY_LIB_OFFLINE_LOGICS_REALISATION
#ifdef EMG_LOGIC_STDOUT
#define COUT_EMG_LOGIC(cout_procedure) cout_procedure;
#else
#define COUT_EMG_LOGIC(cout_procedure) ;
#endif // EMG_LOGIC_STDOUT

#define ToDraw(eIndex_to_draw) emg::Data.Logic.queue_add_to_draw_.psh(eIndex_to_draw)
#define CORRECT_STEP(position) position > WORLD_SIZE-2? 1: position


__forceinline void emg::logics::WorldMamagerTic() {
	lgc::classes::CWorld& world = *static_cast<lgc::classes::CWorld*>(WorldVaribles.wrl);
	for (u_2b offline_indx = 0; offline_indx < ENTITY_LIMIT; ++offline_indx) {
		if (emg::Data.Logic.unit_visual_data_[offline_indx].inst_id < 0) continue; //≈сли логика не обрабатываетс€ подробно
		mth::U2_2pos &unit_pos = emg::Data.Logic.unit_global_step_[offline_indx];
		


	}
	COUT_EMG_LOGIC(_dbg_logic_procedures._dbg.logic_stdout());
}

#undef ToDraw
#undef CORRECT_STEP
#undef COUT_EMG_LOGIC(cout_procedure)

#endif //MY_LIB_OFFLINE_LOGICS_REALISATION