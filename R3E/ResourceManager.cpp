#include "Global.h"

#include "OpenGL.hpp"
#include "ResourceManager.hpp"
#include "Texture.hpp"
#include "ZMS.hpp"
#include "ZMO.hpp"
#include "ZMD.hpp"
#include "Font.hpp"

ResourceManager<ROSE::ZMS> ResourceManager<ROSE::ZMS>::mInstance;
ResourceManager<ROSE::ZMO> ResourceManager<ROSE::ZMO>::mInstance;
ResourceManager<ROSE::ZMD> ResourceManager<ROSE::ZMD>::mInstance;
ResourceManager<Texture> ResourceManager<Texture>::mInstance;
ResourceManager<Font> ResourceManager<Font>::mInstance;
