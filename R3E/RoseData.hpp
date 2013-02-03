#ifndef ROSE_DATA_HPP
#define ROSE_DATA_HPP

#include "ZMD.hpp"
#include "SmartPointer.hpp"
#include "RoseConstants.hpp"

namespace ROSE {
	class Data {
	public:
		static void Load();

	public:
		static SmartPointer<ROSE::ZMD> mAvatarSkeletons[G_MAX];
	};
};

#endif