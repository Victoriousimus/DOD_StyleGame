namespace lgc::classes {

#ifdef WORLD_SIZE
	class CWorld : public lgc::IWorld {
#define	SMngr mth::SI2b_ManagerList
#define SList mth::SI2b_ForwardList
	private:
		struct CRegion {
			t_sym* Cells_[REGION_SIZE];
			SList Units_;
			t_sym Typ_;
			friend lgc::IWorld;
			i_2b Init(mth::U2_2pos& pos, img::CStbImg& _img_ptr) {
				DBGONLY(if (pos.x > REGION_SIZE || pos.y > REGION_SIZE) { return -1; });
				if (!Cells_[pos.x]) Cells_[pos.y] = new t_sym[REGION_SIZE];
				return 0;
			}
			CRegion() : Typ_{0}{
				for (u_2b ic = 0; ic < REGION_SIZE; ++ic) 
					Cells_[ic] = nullptr; 
			}
			~CRegion() {
				for (u_2b ic = 0; ic < REGION_SIZE; ++ic) if (Cells_[ic]) delete[] Cells_;
				SMngr deletemngr;
				deletemngr.Attach(&Units_);
				deletemngr.deleteList();
			}
		};
		CRegion** Regions_[WORLD_SIZE];//*Regions_[WORLD_SIZE][WORLD_SIZE]
		SMngr UnitListManager_;
	public:
		void Print() override {
			std::cout << std::endl;
			std::cout << std::hex << "     ";
			for (i_4b i = 0; i < WORLD_SIZE; ++i) {
				std::cout << std::hex << i << (i > 15 ? "|" : " |");
			}
			std::cout << std::endl << std::endl;
			for (i_4b j, i = 0; i < WORLD_SIZE; ++i) {
				std::cout << i << (i > 15 ? ")" : ") ");
				for (j = 0; j < WORLD_SIZE; ++j) {
					if (Regions_[j][(WORLD_SIZE - 1) - i]) {
						Regions_[j][(WORLD_SIZE - 1) - i]->Typ_;
					}
				}
				std::cout << std::endl;
			}
			std::cout << std::dec << std::endl;
		}
		i_2b Del(mth::U2_2pos& pos) override {
			if (pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE) return -1;
			if (!Regions_[pos.x][pos.y]) return -2;
			delete Regions_[pos.x][pos.y];
			Regions_[pos.x][pos.y] = nullptr;
			return 0;
		}
		i_2b Set(mth::U2_2pos& pos, img::CStbImg& _img_ptr)  override {
			if (pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE) return -1;
			if (!Regions_[pos.x][pos.y]) Regions_[pos.x][pos.y] = new CRegion();
			Regions_[pos.x][pos.y]->Init(pos, _img_ptr);
			return 0;
		}
		SMngr* Entyes(mth::U2_2pos& pos) override {
			if (pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE) return nullptr;
			if (!Regions_[pos.x][pos.y]) return nullptr;
			UnitListManager_.Attach( &(Regions_[pos.x][pos.y]->Units_) );
			return &UnitListManager_;
		}
		t_sym  Region(mth::U2_2pos& pos) override {
			if (pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE) return '\0';
			if (!Regions_[pos.x][pos.y]) return '\0';
			return '.';
		}
		CWorld() {
			for (u_2b j, i = 0; i < WORLD_SIZE; ++i) {
				Regions_[i] = new CRegion*[WORLD_SIZE];
				if (!Regions_[i]) {
					Logger.SetError("ERROR_IN_INIT_WORLD_OBJECT");
					while (i >= 0) { delete[] Regions_[i]; --i; }
					return;
				}
				for (j = 0; j < WORLD_SIZE; ++j) {
					Regions_[i][j] = nullptr;
				}
			}
		}
		~CWorld() {
			for (u_2b j, i = 0; i < WORLD_SIZE; ++i) {
				for (j = 0; j < WORLD_SIZE; ++j) {
					if (Regions_[i][j]) {
						delete Regions_[i][j];
					}
				}
				delete[] Regions_[i];
			}
		}
#undef SMngr
	};
#endif
}