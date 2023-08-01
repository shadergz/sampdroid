
#include <render/rwcore.h>
#include <render/skeleton.h>

#include <log_client.h>

/* render/rwcore.h */
RwCamera* (*RwCameraBeginUpdate)(RwCamera *camera);
RwCamera* (*RwCameraEndUpdate)(RwCamera* camera);
RwCamera* (*RwCameraShowRaster)(RwCamera* camera, void* pDev, RwUInt32 flags);

RwRaster* (*RwRasterCreate)(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags);
RwBool (*RwRasterDestroy)(RwRaster* raster);
RwRaster* (*RwRasterGetOffset)(RwRaster* raster, RwInt16* xOffset, RwInt16* yOffset);
RwInt32 (*RwRasterGetNumLevels)(RwRaster* raster);
RwRaster* (*RwRasterSubRaster)(RwRaster* subRaster, RwRaster* raster, RwRect* rect);
RwRaster* (*RwRasterRenderFast)(RwRaster* raster, RwInt32 x, RwInt32 y);
RwRaster* (*RwRasterRender)(RwRaster* raster, RwInt32 x, RwInt32 y);
RwRaster* (*RwRasterRenderScaled)(RwRaster* raster, RwRect* rect);
RwRaster* (*RwRasterPushContext)(RwRaster* raster);
RwRaster* (*RwRasterPopContext)(void);
RwRaster* (*RwRasterGetCurrentContext)(void);
RwBool (*RwRasterClear)(RwInt32 pixelValue);
RwBool (*RwRasterClearRect)(RwRect* rpRect, RwInt32 pixelValue);
RwRaster* (*RwRasterShowRaster)(RwRaster* raster, void* dev, RwUInt32 flags);
RwUInt8* (*RwRasterLock)(RwRaster* raster, RwUInt8 level, RwInt32 lockMode);
RwRaster* (*RwRasterUnlock)(RwRaster* raster);
RwUInt8* (*RwRasterLockPalette)(RwRaster* raster, RwInt32 lockMode);
RwRaster* (*RwRasterUnlockPalette)(RwRaster* raster);
RwImage* (*RwImageCreate)(RwInt32 width, RwInt32 height, RwInt32 depth);
RwBool (*RwImageDestroy)(RwImage* image);
RwImage* (*RwImageAllocatePixels)(RwImage* image);
RwImage* (*RwImageFreePixels)(RwImage* image);
RwImage* (*RwImageCopy)(RwImage* destImage, const RwImage* sourceImage);
RwImage* (*RwImageResize)(RwImage* image, RwInt32 width, RwInt32 height);
RwImage* (*RwImageApplyMask)(RwImage* image, const RwImage* mask);
RwImage* (*RwImageMakeMask)(RwImage* image);
RwImage* (*RwImageReadMaskedImage)(const RwChar* imageName, const RwChar* maskname);
RwImage* (*RwImageRead)(const RwChar* imageName);
RwImage* (*RwImageWrite)(RwImage* image, const RwChar* imageName);
RwImage* (*RwImageSetFromRaster)(RwImage* image, RwRaster* raster);
RwRaster* (*RwRasterSetFromImage)(RwRaster* raster, RwImage* image);
RwRaster* (*RwRasterRead)(const RwChar* filename);
RwRaster* (*RwRasterReadMaskedRaster)(const RwChar* filename, const RwChar* maskname);
RwImage* (*RwImageFindRasterFormat)(RwImage* ipImage, RwInt32 nRasterType, RwInt32* npWidth, RwInt32* npHeight, RwInt32* npDepth, RwInt32* npFormat);

/* render/rwlpcore.h */
RwReal (*RwIm2DGetNearScreenZ)(void);
RwReal (*RwIm2DGetFarScreenZ)(void);
RwBool (*RwRenderStateGet)(RwRenderState state, void* value);
RwBool (*RwRenderStateSet)(RwRenderState state, void* value);
RwBool (*RwIm2DRenderLine)(RwIm2DVertex* vertices, RwInt32 numVertices, RwInt32 vert1, RwInt32 vert2);
RwBool (*RwIm2DRenderTriangle)(RwIm2DVertex* vertices, RwInt32 numVertices, RwInt32 vert1, RwInt32 vert2, RwInt32 vert3);
RwBool (*RwIm2DRenderPrimitive)(RwPrimitiveType primType, RwIm2DVertex* vertices, RwInt32 numVertices);
RwBool (*RwIm2DRenderIndexedPrimitive)(RwPrimitiveType primType, RwIm2DVertex* vertices, RwInt32 numVertices, RwImVertexIndex* indices, RwInt32 numIndices);

/* render/rtpng.h */
RwImage* (*RtPNGImageWrite)(RwImage* image, const RwChar* imageName);
RwImage* (*RtPNGImageRead)(const RwChar* imageName);

namespace saglobal {
    RsGlobalType* g_rsGlobal;

    extern uintptr_t g_gameAddr;
}

namespace sarender {
    using namespace saglobal;

    void initSaReferences()
    {
        salog::print(salog::Info, "Loading RenderWare Graphics engine functions reference...");

        /* render/skeleton.h */
        g_rsGlobal = (RsGlobalType*)(g_gameAddr + 0xc9b320);
        salog::printFormat(salog::Debug, 
            "1. App Name: %s\n"
            "2. Maximum FPS: %u",
            g_rsGlobal->appName, g_rsGlobal->maxFPS);
        
        /* render/rwcore.h */
        *(uintptr_t**)(&RwCameraBeginUpdate)          = (uintptr_t*)(g_gameAddr + 0x36cf50);
        *(uintptr_t**)(&RwCameraEndUpdate)            = (uintptr_t*)(g_gameAddr + 0x36cf48);
        *(uintptr_t**)(&RwCameraShowRaster)           = (uintptr_t*)(g_gameAddr + 0x36d220);

        *(uintptr_t**)(&RwRasterCreate)               = (uintptr_t*)(g_gameAddr + 0x372e34);
        *(uintptr_t**)(&RwRasterDestroy)              = (uintptr_t*)(g_gameAddr+0);
        *(uintptr_t**)(&RwRasterGetOffset)            = (uintptr_t*)(g_gameAddr + 0x372940);
        *(uintptr_t**)(&RwRasterGetNumLevels)         = (uintptr_t*)(g_gameAddr + 0x372cf4);
        *(uintptr_t**)(&RwRasterSubRaster)            = (uintptr_t*)(g_gameAddr + 0x372da4);
        *(uintptr_t**)(&RwRasterRenderFast)           = (uintptr_t*)(g_gameAddr + 0x372a18);
        *(uintptr_t**)(&RwRasterRender)               = (uintptr_t*)(g_gameAddr + 0x372a9c);
        *(uintptr_t**)(&RwRasterRenderScaled)         = (uintptr_t*)(g_gameAddr + 0x372908);
        *(uintptr_t**)(&RwRasterPushContext)          = (uintptr_t*)(g_gameAddr+0);
        *(uintptr_t**)(&RwRasterPopContext)           = (uintptr_t*)(g_gameAddr + 0x372c8c);
        *(uintptr_t**)(&RwRasterGetCurrentContext)    = (uintptr_t*)(g_gameAddr + 0x3728e4);
        *(uintptr_t**)(&RwRasterClear)                = (uintptr_t*)(g_gameAddr + 0x372994);
        *(uintptr_t**)(&RwRasterClearRect)            = (uintptr_t*)(g_gameAddr + 0x372a60);
        *(uintptr_t**)(&RwRasterShowRaster)           = (uintptr_t*)(g_gameAddr + 0x372d4c);
        *(uintptr_t**)(&RwRasterLock)                 = (uintptr_t*)(g_gameAddr + 0x372f18);
        *(uintptr_t**)(&RwRasterUnlock)               = (uintptr_t*)(g_gameAddr + 0x372954);
        *(uintptr_t**)(&RwRasterLockPalette)          = (uintptr_t*)(g_gameAddr + 0x372c40);
        *(uintptr_t**)(&RwRasterUnlockPalette)        = (uintptr_t*)(g_gameAddr + 0x372ae4);
        *(uintptr_t**)(&RwImageCreate)                = (uintptr_t*)(g_gameAddr + 0x3709d8);
        *(uintptr_t**)(&RwImageDestroy)               = (uintptr_t*)(g_gameAddr + 0x370a50);
        *(uintptr_t**)(&RwImageAllocatePixels)        = (uintptr_t*)(g_gameAddr + 0x370b1c);
        *(uintptr_t**)(&RwImageFreePixels)            = (uintptr_t*)(g_gameAddr + 0x370ad4);
        *(uintptr_t**)(&RwImageCopy)                  = (uintptr_t*)(g_gameAddr + 0x3712f8);
        *(uintptr_t**)(&RwImageResize)                = (uintptr_t*)(g_gameAddr + 0x370c00);
        *(uintptr_t**)(&RwImageApplyMask)             = (uintptr_t*)(g_gameAddr + 0x370f58);
        *(uintptr_t**)(&RwImageMakeMask)              = (uintptr_t*)(g_gameAddr + 0x370dc0);
        *(uintptr_t**)(&RwImageReadMaskedImage)       = (uintptr_t*)(g_gameAddr + 0x371dac);
        *(uintptr_t**)(&RwImageRead)                  = (uintptr_t*)(g_gameAddr + 0x371634);
        *(uintptr_t**)(&RwImageWrite)                 = (uintptr_t*)(g_gameAddr + 0x371cdc);
        *(uintptr_t**)(&RwImageSetFromRaster)         = (uintptr_t*)(g_gameAddr + 0x372568);
        *(uintptr_t**)(&RwRasterSetFromImage)         = (uintptr_t*)(g_gameAddr + 0x3725a4);
        *(uintptr_t**)(&RwRasterRead)                 = (uintptr_t*)(g_gameAddr + 0x37275c);
        *(uintptr_t**)(&RwRasterReadMaskedRaster)     = (uintptr_t*)(g_gameAddr + 0x37285c);
        *(uintptr_t**)(&RwImageFindRasterFormat)      = (uintptr_t*)(g_gameAddr + 0x3725e0);

        /* render/rwlpcore.h */
        *(uintptr_t**)(&RwIm2DGetNearScreenZ)         = (uintptr_t*)(g_gameAddr + 0x37d640);
        *(uintptr_t**)(&RwIm2DGetFarScreenZ)          = (uintptr_t*)(g_gameAddr + 0x37d654);
        *(uintptr_t**)(&RwRenderStateGet)             = (uintptr_t*)(g_gameAddr + 0x37d6b8);
        *(uintptr_t**)(&RwRenderStateSet)             = (uintptr_t*)(g_gameAddr + 0x37d668);
        *(uintptr_t**)(&RwIm2DRenderLine)             = (uintptr_t*)(g_gameAddr + 0x37d6cc);
        *(uintptr_t**)(&RwIm2DRenderTriangle)         = (uintptr_t*)(g_gameAddr + 0x37d6e0);
        *(uintptr_t**)(&RwIm2DRenderPrimitive)        = (uintptr_t*)(g_gameAddr + 0x37d6f4);
        *(uintptr_t**)(&RwIm2DRenderIndexedPrimitive) = (uintptr_t*)(g_gameAddr + 0x37d708);

        /* render/rtpng.h */
        *(uintptr_t**)(&RtPNGImageWrite)              = (uintptr_t*)(g_gameAddr + 0x3ad4d0);
        *(uintptr_t**)(&RtPNGImageRead)               = (uintptr_t*)(g_gameAddr + 0x3ad8c8);
    }

}
