#pragma once
#ifndef MY_LIB_ONLINE_LOGICS_REALISATION
#define MY_LIB_ONLINE_LOGICS_REALISATION
#define Curs emg::Data.Logic.online_manager_list_
#define Queue emg::Data.Logic.queue_add_to_draw_
#define ToDraw(eIndexToDraw) { emg::Data.Render.essence_add_to_show_id_ = eIndexToDraw; emg::render::AddInScene_on_ShowID(); }
#define ToHide(eIndexToDraw) { emg::Data.Render.essence_add_to_hide_id_ = eIndexToDraw; emg::render::DelInScene_on_HideID(); }


enum WAYMASK {
	X = mth::SInstnOffSet::WAYMASK::X,
	Y = mth::SInstnOffSet::WAYMASK::Y,
	Z = mth::SInstnOffSet::WAYMASK::Z
};
__forceinline Bool online_process(i_2b& _indx) {
	i_2b& inst_id = emg::Data.Logic.unit_visual_data_[_indx].inst_id;
	i_2b& anim_id = emg::Data.Logic.unit_visual_data_[_indx].anim_id;
	mth::SInstnOffSet& offSet = emg::Data.Render.InstnOffSets_[inst_id];
	mth::SInstnAttrib& attrib = emg::Data.Render.InstnAttribs_[inst_id];
	mth::F4_3pos& last_pos = emg::Data.Render.InstnEndPoints_[inst_id];
	offSet.VectorPos = WAYMASK::X;
	offSet.VectorRot = WAYMASK::Z;
	offSet.VeloPos = 0.003f;
	offSet.VeloRot = 0.001f;
	return false;
}

__forceinline void emg::logics::Online_world_iteration() {
	i_2b enty_indx = 0;
	Bool hide_flag;
	Curs.First();
	while ((enty_indx = Queue.Pop()) > 0) {
		ToDraw(enty_indx);
		Curs.PushItem(enty_indx);
	}
	while ((enty_indx = Curs.DataItem()) > 0){
		Bool hide_flag = online_process(enty_indx);
		if (hide_flag) {
			ToHide(enty_indx);
			Curs.RemoveItem();
		} else {
			if (Curs.NextItem() < 0) break;;
		}
	}
}
#undef Curs
#undef Queue
#undef ToDraw
#undef ToHide

#endif // MY_LIB_OFFLINE_LOGICS_REALISATION