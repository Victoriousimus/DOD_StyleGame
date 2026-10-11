#pragma once
#ifndef _EMG_MATH_ATTACH_
#define _EMG_MATH_ATTACH_
#include <math.h>

#define METER_TO_FLOAT 0.000247f
#define MTH_PI 3.1415926f		//Число PI
#define MTH_ANGLE_R 0.0174532f	//Угловой коэфициент PI/180
#define MTH_ANGLE_A 57,295779f	//Угловой коэфициент 180/PI
#define MTH_RADIANS 6.2831853f	//Число 2 * PI
#define MTH_MIN_ANG_Y -1.57075f	//Число -PI/2
#define MTH_MAX_ANG_Y  1.57075f	//Число  PI/2
#define MTH_ANG360(ang) ((int)ang % 360) + (ang - (int)ang)					//Приведение градуированного угла
#define MTH_ANGRAD(ang) (f_4b)((int)(ang*10000000) % 62831853)/10000000.0f	//Приведение радиального угла
#define MTH_ADDRAD(ang) ang += (ang < 0 ? SPT_RADIANS : (ang >= SPT_RADIANS ? -SPT_RADIANS : 0.01f));

typedef void* PMem;
typedef const char* c_wrd;
typedef char* t_wrd;
typedef const char c_sym;
typedef char t_sym;
typedef GLboolean Bool;
typedef GLfloat f_4b;
typedef GLdouble f_8b;
typedef int8_t i_1b;
typedef uint8_t u_1b;
typedef int16_t i_2b;
typedef uint16_t u_2b;
typedef int32_t i_4b;
typedef uint32_t u_4b;
typedef int64_t i_8b;
typedef uint64_t u_8b;
typedef glm::vec2 v2_8b;
typedef glm::vec3 v3_12b;
typedef glm::vec4 v4_16b;
typedef glm::mat4 m4_64b;


namespace mth {
	struct U2_2pos {
		u_2b x;
		u_2b y;
	};
	struct F4_3pos {
		f_4b x;
		f_4b y;
		f_4b z;
	};
	struct F4_2pos {
		f_4b x;
		f_4b y;
	};
	struct F8_2pos {
		f_8b x;
		f_8b y;
	};

	struct SEssenceMeta {
		u_2b txtr_id; //( Draw_Texture_ID  ) Номер текстуры
		i_2b anim_id; //( Draw_Animated_ID ) Номер анимации
		i_2b mesh_id; //( Draw_Command_ID  ) Номер модели 
		i_2b inst_id; //( Draw_Instance_ID ) Номер инстанса
		f_4b Atlas_X;
		f_4b Atlas_Y;
	};
	struct SInstnOffSet {
		enum WAYMASK : u_4b {
			X = 0b00000000000000000000001111111111u,
			Y = 0b00000000000011111111110000000000u,
			Z = 0b00111111111100000000000000000000u
		};
		f_4b VeloPos;
		f_4b VeloRot;
		u_4b VectorPos;
		u_4b VectorRot;
		void GetVelosityPos(F4_3pos* resultPos) {
			resultPos->x = (VectorPos & WAYMASK::X) > 0? VeloPos : 0.0f;
			resultPos->y = (VectorPos & WAYMASK::Y) > 0? VeloPos : 0.0f;
			resultPos->z = (VectorPos & WAYMASK::Z) > 0? VeloPos : 0.0f;
		}
		void GetVelosityRot(F4_3pos* resultRot) {
			resultRot->x = (VectorRot & WAYMASK::X) > 0 ? VeloRot : 0.0f;
			resultRot->y = (VectorRot & WAYMASK::Y) > 0 ? VeloRot : 0.0f;
			resultRot->z = (VectorRot & WAYMASK::Z) > 0 ? VeloRot : 0.0f;
		}
	};
	struct SInstnAttrib {
		f_4b pich;
		f_4b yaw;
		f_4b roll;
		f_4b x;
		f_4b z;	//}{0ba
		f_4b y;	//}{0ba
		f_4b Atlas_X;
		f_4b Atlas_Y;
	};
	//Структура команды отрисовки DrawArraysIndirectCommand = 128_bit
	struct SDrawCommand {
		u_4b vert_count;	// Количество вершин для примитива
		u_4b inst_count;	// Количество инстансов
		u_4b vert_first;	// Начальный индекс в вершинном массиве
		u_4b inst_first;	// Базовый инстанс (добавляется к gl_InstanceID)
	};

#ifdef MTH_ARR_QUEUE
	struct SI2b_ArrQueue {
	private:
		i_2b arr[MTH_ARR_QUEUE];
		u_2b inp;
		u_2b out;
		u_2b lim;
	public:
		SI2b_ArrQueue() : inp(0), out(0), lim(MTH_ARR_QUEUE - 1) {
			#ifdef _DEBUG
			for (u_2b i = 0; i < MTH_ARR_QUEUE; ++i) { arr[i] = -1; }
			#endif
		}
		~SI2b_ArrQueue() {}
		i_2b Push(i_2b _data) {
			if (inp < lim) {
				if (inp + 1 == out) return -1;
				arr[inp] = _data; ++inp;
				return 0;
			}
			if (out == 0) return -1;
			arr[inp] = _data; inp = 0;
			return 0;
		}
		i_2b Pop() {
			if (out == inp) return -1;
			if (out < lim) {
				#ifdef _DEBUG
					i_2b tpm = arr[out];
					arr[out] = -2;
					++out;
					return tpm;
				#else
					++out;
					return arr[out - 1];
				#endif
			}
			if (inp == 0) return -1;
			out = 0;
			return arr[lim];
		}
#ifdef _DEBUG
		__forceinline void Print() {
			std::cout << "QUEUE_ARRAY:" << std::endl;
			for (u_2b i = 0; i < MTH_ARR_QUEUE; ++i) {
				std::cout 
					<< "\t[" << i << "]\t  value = " << std::dec << arr[i] 
					<< (i==inp? "  <- INP" : "")
					<< (i==out? "  <- OUT" : "")
					<< std::endl;
			}
			std::cout << std::endl;
		}
#endif
	};
#endif // MTH_ARR_QUEUE

#ifdef MTH_LIST_STRCT
	struct SI2b_ManagerList;
	struct SI2b_ForwardList {
	private:
		struct SItem {
			i_2b value;
			SItem* next;
			SItem(const i_2b& _val, SItem* _next = nullptr) : value(_val), next(_next) {}
			~SItem() { value = -1; next = nullptr; }
		};
		SItem* lst;
		void Clear() {
			if (lst == nullptr) return;
			while (lst != nullptr) {
				SItem* tmp = lst;
				lst = lst->next;
				delete tmp;
			}
		}
		friend SI2b_ManagerList;
	public:
		SI2b_ForwardList(): lst(nullptr) {}
		~SI2b_ForwardList() { Clear(); }
	};

	struct SI2b_ManagerList {
	private:
		SI2b_ForwardList* list_ptr;
		SI2b_ForwardList::SItem** cur;
    public:
		SI2b_ManagerList() : list_ptr(nullptr), cur(nullptr) {}
        ~SI2b_ManagerList() { Release(); }
		__forceinline void Attach(SI2b_ForwardList* _list_ref) {
			list_ptr = _list_ref;
			cur = &(list_ptr->lst);
		}
		__forceinline void Release() {
			list_ptr = nullptr; 
			cur = nullptr;
		}
		__forceinline void First() {
			if (list_ptr == nullptr) return;
			cur = &(list_ptr->lst);
		}
		__forceinline void deleteList() {
			if (list_ptr == nullptr) return;
			list_ptr->Clear();
		}
		__forceinline i_2b PushItem(const i_2b& value) {
			if (cur == nullptr) {
				return -1;
			}
			list_ptr->lst = new SI2b_ForwardList::SItem(value, list_ptr->lst);
			return 0;
		}
		__forceinline void RemoveItem() {
			if (cur == nullptr) return;
			if (*cur == nullptr) return;
			SI2b_ForwardList::SItem* to_delete = *cur;
			*cur = to_delete->next;
			delete to_delete;
        }
		__forceinline i_2b NextItem() {
			if (cur == nullptr) return-1;
            if ((*cur)->next == nullptr) return-1;
            cur = &((*cur)->next);
            return 0;
        }
		__forceinline i_2b DataItem() {
			if (cur == nullptr) return -1;
            if (*cur == nullptr) return -1;
            return (*cur)->value;
        }

#ifdef _DEBUG
		__forceinline void Print() {
			if (list_ptr == nullptr) {
				std::cout << "\t< Ox" << std::hex << (u_8b)&list_ptr
					<< " > List_Manager _data< Ox" << std::hex << (u_8b)list_ptr << " >" << std::endl;
				std::cout << "\t< Ox" << std::hex << (u_8b)&cur
					<< " > List_Manager _curs< Ox" << std::hex << (u_8b)cur << " >" << std::endl;
				return;
			}
			if (list_ptr->lst == nullptr) {
				std::cout << "\t< Ox" << std::hex << (u_8b)&list_ptr
					<< " > List_Manager _data< Ox" << std::hex << (u_8b)list_ptr << " >" << std::endl;
				std::cout << "\t< Ox" << std::hex << (u_8b)&cur
					<< " > List_Manager _curs< Ox" << std::hex << (u_8b)cur << " >" << std::endl;
				std::cout << "\t< Ox" << std::hex << (u_8b)&list_ptr->lst
					<< " > List_Source  _head< Ox" << std::hex << (u_8b)list_ptr->lst << " > " << std::endl;
				return;
			}
			u_2b indx = 0;
			SI2b_ForwardList::SItem* tmp = list_ptr->lst;
			std::cout << "\t< Ox" << std::hex << (u_8b)&list_ptr
				<< " > List_Manager _data< Ox" << std::hex << (u_8b)list_ptr << " >" << std::endl;
			std::cout << "\t< Ox" << std::hex << (u_8b)&cur
				<< " > List_Manager _curs< Ox" << std::hex << (u_8b)cur << " >" << std::endl;
			std::cout << "\t< Ox" << std::hex << (u_8b)&list_ptr->lst
				<< " > List_Source  _head< Ox" << std::hex << (u_8b)list_ptr->lst << " > " << std::endl;
			while (tmp != nullptr) {
				std::cout
					<< "\t\t[" << std::dec << indx << "] < Ox" << std::hex << (u_8b)tmp
					<< " > : DATA[ " << std::dec << (i_8b)tmp->value
					<< " ] NEXT[ Ox" << std::hex << (u_8b)tmp->next << " ]";
				if (tmp == list_ptr->lst) std::cout << "\t  <- HEAD";
				if (tmp == *cur) std::cout << "\t  <- CURS";
				std::cout << std::endl;
				tmp = tmp->next; ++indx;
			}
			std::cout << std::dec << std::endl;
		}
#endif // _DEBUG
	};
#endif//MTH_LIST_STRCT
}
#endif //_EMG_MATH_ATTACH_