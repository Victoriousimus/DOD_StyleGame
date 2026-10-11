#pragma once
#pragma once
#ifndef MY_LIB_LOGIC_CLASSES
#define MY_LIB_LOGIC_CLASSES
#define sBIT 4
#define sDEX 16.0f

namespace img {
#ifdef STB_IMAGE_IMPLEMENTATION
	class CStbImg {
	private:
		i_4b x_;
		i_4b y_;
		u_1b* data_;
		Bool empty_;
	public:
		CStbImg() : empty_(true), data_(nullptr), x_(0), y_(0) {}
		~CStbImg() { free(); }
		void free() {
			if (empty_) return;
			empty_ = true; x_ = 0; y_ = 0;
			stbi_image_free(data_);
		}
		i_4b load(c_wrd _fname) {
			if (!empty_) return -1; i_4b n;
			data_ = stbi_load(_fname, &x_, &y_, &n, 3);
			if (!data_) return -1;
			empty_ = false;
			return x_;
		}
		void draw(f_4b _posX, f_4b _posY) {
			Logger.SetError("CStbImg.draw() - NOT_REALISATED IN <classes.h>");
		}
		void getPix(i_2b _X, i_2b _Y, u_4b* color) {
			if (_X >= x_ || _X < 0 || _Y >= y_ || _Y < 0) {
				*color = 0;
				return;
			}
			*color =
				((u_4b)(data_[(_Y * y_ + _X) * 3 + 0]) * 1000000) +
				((u_4b)(data_[(_Y * y_ + _X) * 3 + 1]) * 1000) +
				((u_4b)(data_[(_Y * y_ + _X) * 3 + 2]));
		}
		void getPix(i_2b _X, i_2b _Y, u_1b(*color)[3]) {
			if (_X >= x_ || _X < 0 || _Y >= y_ || _Y < 0) {
				(*color)[2] = 0;
				(*color)[1] = 0;
				(*color)[0] = 0;
				return;
			}
			std::memcpy(*color, &(data_[(_Y * y_ + _X) * 3]), 3);
		}
		i_4b setPix(i_2b _X, i_2b _Y, u_1b(&color)[3]) {
			if (_X >= x_ || _X < 0 || _Y >= y_ || _Y < 0) return -1;
			std::memcpy(&(data_[(_Y * y_ + _X) * 3]), color, 3);
			return 0;
		}
	};
#endif // !STB_IMAGE_IMPLEMENTATION
}

#endif