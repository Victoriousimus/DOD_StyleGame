#pragma once
namespace lgc::classes {
	class CTerrain : public lgc::ITerrain {
	private:
		u_1b** HeightMap_;
		mth::SI2b_ForwardList units;
	public:
		CTerrain() : HeightMap_(nullptr) { size_ = 0; }
		~CTerrain() { Free(); }
		u_1b Typ(mth::U2_2pos& pos) override {
			return HeightMap_[pos.x][pos.y];
		}
		void Init(img::CStbImg& image, u_2b size) override {
		}
		void Free() override {
			if (!size_) return;
			for (i_4b x = 0; x < size_; ++x) { delete[] HeightMap_[x]; }
			delete[] HeightMap_;
			size_ = 0;
		}
		void Print() override {
			std::cout << std::endl;
			for (i_4b x, y = size_ - 1; y > -1; --y) {
				for (x = 0; x < size_; ++x) { std::cout << " " << HeightMap_[y][x] << " "; }
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
	};
}