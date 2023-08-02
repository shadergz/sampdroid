#include <algorithm>
#include <vector>

#include <log_client.h>
#include <jvm_helper.h>

#include <render/rwlpcore.h>
#include <render/skeleton.h>

namespace saglobal {
    extern RsGlobalType* g_rsGlobal;
    extern uintptr_t g_gameAddr;
}

std::vector<RwIm2DVertex> vertexBuffer;

void ImGui_ImplRenderWare_RenderDrawData([[maybe_unused]] ImDrawData* drawData)
{
    salog::print(salog::Info, "Impl IMGUI RenderWare for data draw purposes, has been called");

    const RwReal* nearScreenZ{reinterpret_cast<RwReal*>(saglobal::g_gameAddr + 0xe20868)};
    const RwReal* recipNearClip{reinterpret_cast<RwReal*>(saglobal::g_gameAddr + 0xe20864)};

    if (vertexBuffer.size() < drawData->TotalVtxCount)
        vertexBuffer.resize(drawData->TotalVtxCount);
    
    RwIm2DVertex* vtxDest{vertexBuffer.data()};
    uint32_t vtxOffset{0};

    // Fill up the entire vertex buffer and fix all screen coordinates
    for (uint32_t cmdIdx = 0; cmdIdx < drawData->CmdListsCount; cmdIdx++) {
        const ImDrawList* cmdList{drawData->CmdLists[cmdIdx]};

        std::for_each(cmdList->VtxBuffer.begin(), cmdList->VtxBuffer.end(), 
            [&vtxDest, nearScreenZ, recipNearClip](const ImDrawVert& vertexPos){
            // Fixes all screen coordinates to perfectly draw all vertices onto it
            RwIm2DVertexSetScreenX(vtxDest, vertexPos.pos.x);
            RwIm2DVertexSetScreenY(vtxDest, vertexPos.pos.y);
            RwIm2DVertexSetScreenZ(vtxDest, *nearScreenZ);
            
            RwIm2DVertexSetRecipCameraZ(vtxDest, *recipNearClip);
            vtxDest->emissiveColor = vertexPos.col;
            
            RwIm2DVertexSetU(vtxDest, vertexPos.uv.x, recipCameraZ);
            
            RwIm2DVertexSetV(vtxDest, vertexPos.uv.y, recipCameraZ);

            vtxDest++;
        });

        ImDrawIdx* idxBuffer{cmdList->IdxBuffer.Data};

        std::for_each(cmdList->CmdBuffer.begin(), cmdList->CmdBuffer.end(), 
            [cmdList, &idxBuffer, vtxOffset](const ImDrawCmd& cmdBuffer){
            if (cmdBuffer.UserCallback) {
                cmdBuffer.UserCallback(cmdList, &cmdBuffer);
            } else {
                static ImVec4 scissorRect{};
                scissorRect.x = cmdBuffer.ClipRect.x;
                scissorRect.y = cmdBuffer.ClipRect.y;
                scissorRect.z = cmdBuffer.ClipRect.z;
                scissorRect.w = cmdBuffer.ClipRect.w;

                RwRenderStateSet(rwRENDERSTATEZTESTENABLE, nullptr);
                RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, nullptr);
                RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void*>(1));
                RwRenderStateSet(rwRENDERSTATESRCBLEND, reinterpret_cast<void*>(rwBLENDSRCALPHA));
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, reinterpret_cast<void*>(rwBLENDINVSRCALPHA));
                RwRenderStateSet(rwRENDERSTATEFOGENABLE, nullptr);
                RwRenderStateSet(rwRENDERSTATECULLMODE, reinterpret_cast<void*>(rwCULLMODECULLNONE));
                RwRenderStateSet(rwRENDERSTATEBORDERCOLOR, nullptr);
                RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, reinterpret_cast<void*>(rwALPHATESTFUNCTIONGREATER));
                RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, reinterpret_cast<void*>(2));
                RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, reinterpret_cast<void*>(rwFILTERLINEAR));
                RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, reinterpret_cast<void*>(rwTEXTUREADDRESSCLAMP));
                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, reinterpret_cast<void*>(cmdBuffer.TextureId));

                // Draw all primitives from this vertex buffer position
                RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, 
                    &vertexBuffer[vtxOffset], static_cast<RwInt32>(cmdList->VtxBuffer.Size),
                    reinterpret_cast<RwImVertexIndex*>(idxBuffer), 
                    cmdBuffer.ElemCount);
                
                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
                std::memset(&scissorRect, 0, sizeof scissorRect);
            }

            idxBuffer += cmdBuffer.ElemCount;
        });
        vtxOffset += cmdList->VtxBuffer.Size;
    }
}

bool ImGui_ImplRenderWare_Init()
{
    auto& io{ImGui::GetIO()};
    const ImVec2 displaySz(static_cast<float>(saglobal::g_rsGlobal->maximumWidth), 
        static_cast<float>(saglobal::g_rsGlobal->maximumHeight));
    
    if (displaySz.x && displaySz.y) {
        io.DisplaySize = displaySz;
    } else {
        io.DisplaySize = sajvm::getScreenSize();
    }
    // Updates to the screen size will only appear when the next frame buffer is rendered
    ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);
    
    salog::printFormat(salog::Info, "GUI: Client display size: width = (%f), height = (%f)", 
        io.DisplaySize.x, io.DisplaySize.y);

    return true;
}

static RwRaster* fontRaster{nullptr};
static RwImage* fontPixels{};

static void ImGui_ImplRenderWare_CreateDeviceObjects()
{
    salog::printFormat(salog::Info, "GUI: Creating new device objects");
    auto& io{ImGui::GetIO()};
    uint8_t* pxS;
    
    RwInt32 outWidth,
        outHeight, outBytesPerPixel;
    
    io.Fonts->GetTexDataAsRGBA32(&pxS, &outWidth, &outHeight, &outBytesPerPixel);
    outBytesPerPixel *= 8;

    salog::printFormat(salog::Info, "GUI: Font atlas pixels (%#lp), width (%d), height (%d)",
        pxS, outWidth, outHeight);
    
    RwImage* fontData{RwImageCreate(outWidth, outHeight, outBytesPerPixel)};
    salog::printFormat(salog::Info, "Font data allocated at %#p", fontData);

    if (!fontData) {
        salog::print(salog::Error, "RW couldn't allocate the desired image, quitting...");
        std::terminate();
    }
    
    fontPixels = RwImageAllocatePixels(fontData);
    /*
    struct RwImage {
        RwInt32 flags, width, height, depth, stride;
        RwUInt8 *cpPixels;
        RwRGBA *palette;
    };
    */

    RwUInt8* fontPixelsDest{fontData->cpPixels};

    for (uint32_t strideIdx{0}; strideIdx < fontData->height; strideIdx++) {
        memcpy(fontPixelsDest, pxS + fontData->stride * strideIdx, fontData->stride);
        fontPixelsDest += fontData->stride;
    }

    RwInt32 width, 
        height,
        delph,
        flags;
    
    RwImageFindRasterFormat(fontData, rwRASTERTYPETEXTURE, &width, &height, &delph, &flags);
    RwRaster* preFontImage{RwRasterCreate(width, height, delph, flags)};

    fontRaster = RwRasterSetFromImage(preFontImage, fontData);
    salog::printFormat(salog::Info, "Font raster in %#p by %#p", fontRaster, fontPixels);

    RwImageDestroy(fontData);

    io.Fonts->TexID = reinterpret_cast<ImTextureID*>(fontRaster);

}

void ImGui_ImplRenderWare_Shutdown()
{
    auto& io{ImGui::GetIO()};
    // Destroy font Raster data
    RwRasterDestroy(fontRaster);
    // The font pixels are no longer needed
    RwImageFreePixels(fontPixels);
    fontPixels = nullptr;

    fontRaster = nullptr;
    io.Fonts->TexID = nullptr;

    vertexBuffer.resize(0);

}

void ImGui_ImplRenderWare_NewFrame()
{
    if (!fontRaster)
        ImGui_ImplRenderWare_CreateDeviceObjects();

    [[maybe_unused]] auto& io{ImGui::GetIO()};   
}

