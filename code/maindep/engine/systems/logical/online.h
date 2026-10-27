#pragma once
#ifndef MY_LIB_ONLINE_LOGICS_REALISATION
#define MY_LIB_ONLINE_LOGICS_REALISATION
#define Curs emg::Data.Logic.online_manager_list_
#define Queue emg::Data.Logic.queue_add_to_draw_
#define ToDraw(eIndexToDraw) { emg::Data.Render.essence_add_to_show_id_ = (eIndexToDraw); emg::render::InsertInDrawPipline(); }
#define ToHide(eIndexToDraw) { emg::Data.Render.essence_add_to_hide_id_ = (eIndexToDraw); emg::render::RemoveInDrawPipline(); }

enum WAYMASK {
	X = mth::SInstnOffSet::WAYMASK::X,
	Y = mth::SInstnOffSet::WAYMASK::Y,
	Z = mth::SInstnOffSet::WAYMASK::Z
};
__forceinline Bool online_process(i_2b& _indx) {

	return false;
}

__forceinline void emg::logics::LocalManagerTic() {
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