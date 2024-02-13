#include <rwcore.h>

#define RWRGBALONG(r,g,b,a) ((unsigned int) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

extern RwImage* (*RtPNGImageWrite)(RwImage *image, const RwChar *imageName);
extern RwImage* (*RtPNGImageRead)(const RwChar *imageName);

