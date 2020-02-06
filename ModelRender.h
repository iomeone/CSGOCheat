#pragma once

#include "Material.h"
#include "RandomStuff.h"
typedef unsigned __int32 uint32;
typedef unsigned short ModelInstanceHandle_t;
typedef void* StudioDecalHandle_t;

enum OverrideType_t
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
};

struct LightCacheHandle_t;
struct DrawModelInfo_t;
struct model_t;
struct DrawModelState_t;
struct MaterialLightingState_t;
struct StaticPropRenderInfo_t;
struct ColorMeshInfo_t;
struct LightingQuery_t;
struct DataCacheHandle_t;
struct StaticLightingQuery_t;
struct Ray_t;


struct ModelRenderInfo_t
{
	Vector origin;
	Vector angles;
	char pad[0x4]; 
	void *pRenderable;
	const void *pModel;
	const matrix3x4_t *pModelToWorld;
	const matrix3x4_t *pLightingOffset;
	const Vector *pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;

	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};

class QAngle;
class IClientRenderable;
class IMesh;
class Vector4D;
class VMatrix;
class IMorph;
class ICallQueue;
struct Rect_t;
struct OcclusionQueryObjectHandle_t;
typedef unsigned int MorphFormat_t;
class IVertexBuffer;
class IIndexBuffer;
class CStudioHdr;
enum NormalDecodeMode_t
{
	NORMAL_DECODE_NONE = 0,
	NORMAL_DECODE_ATI2N = 1,
	NORMAL_DECODE_ATI2N_ALPHA = 2
};
class IVTFTexture;
class ITextureRegenerator
{
public:
	// This will be called when the texture bits need to be regenerated.
	// Use the VTFTexture interface, which has been set up with the
	// appropriate texture size + format
	// The rect specifies which part of the texture needs to be updated
	// You can choose to update all of the bits if you prefer
	virtual void RegenerateTextureBits(ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pRect) = 0;

	// This will be called when the regenerator needs to be deleted
	// which will happen when the texture is destroyed
	virtual void Release() = 0;
};
class ITexture
{
public:
	// Various texture polling methods
	virtual const char *GetName(void) const = 0;
	virtual int GetMappingWidth() const = 0;
	virtual int GetMappingHeight() const = 0;
	virtual int GetActualWidth() const = 0;
	virtual int GetActualHeight() const = 0;
	virtual int GetNumAnimationFrames() const = 0;
	virtual bool IsTranslucent() const = 0;
	virtual bool IsMipmapped() const = 0;

	virtual void GetLowResColorSample(float s, float t, float *color) const = 0;

	// Gets texture resource data of the specified type.
	// Params:
	//		eDataType		type of resource to retrieve.
	//		pnumBytes		on return is the number of bytes available in the read-only data buffer or is undefined
	// Returns:
	//		pointer to the resource data, or NULL
	virtual void *GetResourceData(uint32 eDataType, size_t *pNumBytes) const = 0;

	// Methods associated with reference count
	virtual void IncrementReferenceCount(void) = 0;
	virtual void DecrementReferenceCount(void) = 0;

	inline void AddRef() { IncrementReferenceCount(); }
	inline void Release() { DecrementReferenceCount(); }

	// Used to modify the texture bits (procedural textures only)
	virtual void SetTextureRegenerator(ITextureRegenerator *pTextureRegen) = 0;

	// Reconstruct the texture bits in HW memory

	// If rect is not specified, reconstruct all bits, otherwise just
	// reconstruct a subrect.
	virtual void Download(Rect_t *pRect = 0, int nAdditionalCreationFlags = 0) = 0;

	// Uses for stats. . .get the approximate size of the texture in it's current format.
	virtual int GetApproximateVidMemBytes(void) const = 0;

	// Returns true if the texture data couldn't be loaded.
	virtual bool IsError() const = 0;

	// NOTE: Stuff after this is added after shipping HL2.

	// For volume textures
	virtual bool IsVolumeTexture() const = 0;
	virtual int GetMappingDepth() const = 0;
	virtual int GetActualDepth() const = 0;

	virtual ImageFormat GetImageFormat() const = 0;
	virtual NormalDecodeMode_t GetNormalDecodeMode() const = 0;

	// Various information about the texture
	virtual bool IsRenderTarget() const = 0;
	virtual bool IsCubeMap() const = 0;
	virtual bool IsNormalMap() const = 0;
	virtual bool IsProcedural() const = 0;

	virtual void DeleteIfUnreferenced() = 0;

	// swap everything except the name with another texture
	virtual void SwapContents(ITexture *pOther) = 0;

	// Retrieve the vtf flags mask
	virtual unsigned int m_fFlags(void) const = 0;

	// Force LOD override (automatically downloads the texture)
	virtual void ForceLODOverride(int iNumLodsOverrideUpOrDown) = 0;

	// Save texture to a file.
	virtual bool SaveToFile(const char *fileName) = 0;
};

class IRefCounted
{
public:
	virtual int AddRef() = 0;
	virtual int Release() = 0;
};

struct LightDesc_t;


enum StencilOperation_t
{
	STENCILOPERATION_KEEP = 1,
	STENCILOPERATION_ZERO = 2,
	STENCILOPERATION_REPLACE = 3,
	STENCILOPERATION_INCRSAT = 4,
	STENCILOPERATION_DECRSAT = 5,
	STENCILOPERATION_INVERT = 6,
	STENCILOPERATION_INCR = 7,
	STENCILOPERATION_DECR = 8,
	STENCILOPERATION_FORCE_DWORD = 0x7fffffff
};

enum StencilComparisonFunction_t
{
	STENCILCOMPARISONFUNCTION_NEVER = 1,
	STENCILCOMPARISONFUNCTION_LESS = 2,
	STENCILCOMPARISONFUNCTION_EQUAL = 3,
	STENCILCOMPARISONFUNCTION_LESSEQUAL = 4,
	STENCILCOMPARISONFUNCTION_GREATER = 5,
	STENCILCOMPARISONFUNCTION_NOTEQUAL = 6,
	STENCILCOMPARISONFUNCTION_GREATEREQUAL = 7,
	STENCILCOMPARISONFUNCTION_ALWAYS = 8,
	STENCILCOMPARISONFUNCTION_FORCE_DWORD = 0x7fffffff
};

enum ShaderStencilOp_t
{
	SHADER_STENCILOP_KEEP = 1,
	SHADER_STENCILOP_ZERO = 2,
	SHADER_STENCILOP_SET_TO_REFERENCE = 3,
	SHADER_STENCILOP_INCREMENT_CLAMP = 4,
	SHADER_STENCILOP_DECREMENT_CLAMP = 5,
	SHADER_STENCILOP_INVERT = 6,
	SHADER_STENCILOP_INCREMENT_WRAP = 7,
	SHADER_STENCILOP_DECREMENT_WRAP = 8,
	SHADER_STENCILOP_FORCE_DWORD = 0x7fffffff
};

enum ShaderStencilFunc_t
{
	SHADER_STENCILFUNC_NEVER = 1,
	SHADER_STENCILFUNC_LESS = 2,
	SHADER_STENCILFUNC_EQUAL = 3,
	SHADER_STENCILFUNC_LEQUAL = 4,
	SHADER_STENCILFUNC_GREATER = 5,
	SHADER_STENCILFUNC_NOTEQUAL = 6,
	SHADER_STENCILFUNC_GEQUAL = 7,
	SHADER_STENCILFUNC_ALWAYS = 8,
	SHADER_STENCILFUNC_FORCE_DWORD = 0x7fffffff
};



struct ShaderStencilState_t {
	bool m_bEnable;
	int m_FailOp;
	int m_ZFailOp;
	int m_PassOp;
	int m_CompareFunc;
	int m_nReferenceValue;
	uint32 m_nTestMask;
	uint32 m_nWriteMask;

	ShaderStencilState_t() {
		m_bEnable = false;
		m_PassOp = m_FailOp = m_ZFailOp = STENCILOPERATION_KEEP;
		m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
		m_nReferenceValue = 0;
		m_nTestMask = m_nWriteMask = 0xFFFFFFFF;
	}
};

enum MaterialMatrixMode_t
{
	MATERIAL_VIEW = 0,
	MATERIAL_PROJECTION,

	// Texture matrices
	MATERIAL_TEXTURE0,
	MATERIAL_TEXTURE1,
	MATERIAL_TEXTURE2,
	MATERIAL_TEXTURE3,
	MATERIAL_MODEL,
	// Total number of matrices
	NUM_MATRIX_MODES = MATERIAL_MODEL + 1,

	// Number of texture transform
	NUM_TEXTURE_TRANSFORMS = MATERIAL_TEXTURE3 - MATERIAL_TEXTURE0 + 1
};
enum MaterialCullMode_t
{
	MATERIAL_CULLMODE_CCW,	// this culls polygons with counterclockwise winding
	MATERIAL_CULLMODE_CW	// this culls polygons with clockwise winding
};
enum MaterialHeightClipMode_t
{
	MATERIAL_HEIGHTCLIPMODE_DISABLE,
	MATERIAL_HEIGHTCLIPMODE_RENDER_ABOVE_HEIGHT,
	MATERIAL_HEIGHTCLIPMODE_RENDER_BELOW_HEIGHT
};
enum MaterialFogMode_t
{
	MATERIAL_FOG_NONE,
	MATERIAL_FOG_LINEAR,
	MATERIAL_FOG_LINEAR_BELOW_FOG_Z,
};
struct FlashlightState_t
{
	Vector m_vecLightDirection; // FLASHLIGHTFIXME: can get this from the matrix
	Vector m_vecLightOrigin;  // FLASHLIGHTFIXME: can get this from the matrix
	float m_NearZ;
	float m_FarZ;
	float m_fHorizontalFOVDegrees;
	float m_fVerticalFOVDegrees;
	float m_fQuadraticAtten;
	float m_fLinearAtten;
	float m_fConstantAtten;
	Vector m_Color;
	ITexture *m_pSpotlightTexture;
	int m_nSpotlightTextureFrame;
	bool  m_bEnableShadows;
};
enum MorphWeightType_t
{
	MORPH_WEIGHT = 0,
	MORPH_WEIGHT_LAGGED,
	MORPH_WEIGHT_STEREO,
	MORPH_WEIGHT_STEREO_LAGGED,

	MORPH_WEIGHT_COUNT,
};
struct MorphWeight_t
{
	float m_pWeight[MORPH_WEIGHT_COUNT];
};
enum MaterialIndexFormat_t
{
	MATERIAL_INDEX_FORMAT_UNKNOWN = -1,
	MATERIAL_INDEX_FORMAT_16BIT = 0,
	MATERIAL_INDEX_FORMAT_32BIT,
};
enum MaterialPrimitiveType_t 
{ 
	MATERIAL_POINTS			= 0x0,
	MATERIAL_LINES,
	MATERIAL_TRIANGLES,
	MATERIAL_TRIANGLE_STRIP,
	MATERIAL_LINE_STRIP,
	MATERIAL_LINE_LOOP,	// a single line loop
	MATERIAL_POLYGON,	// this is a *single* polygon
	MATERIAL_QUADS,
	MATERIAL_INSTANCED_QUADS, // (X360) like MATERIAL_QUADS, but uses vertex instancing

	// This is used for static meshes that contain multiple types of
	// primitive types.	When calling draw, you'll need to specify
	// a primitive type.
	MATERIAL_HETEROGENOUS
};



class IMatRenderContext : public IRefCounted
{
public:
	virtual void				BeginRender() = 0;
	virtual void				EndRender() = 0;

	virtual void				Flush(bool flushHardware = false) = 0;

	virtual void				BindLocalCubemap(ITexture *pTexture) = 0;

	// pass in an ITexture (that is build with "rendertarget" "1") or
	// pass in NULL for the regular backbuffer.
	virtual void				SetRenderTarget(ITexture *pTexture) = 0;
	virtual ITexture *			GetRenderTarget() = 0;

	virtual void				GetRenderTargetDimensions(int &width, int &height) const = 0;

	// Bind a material is current for rendering.
	virtual void				Bind(IMaterial *material, void *proxyData = nullptr) = 0;
	// Bind a lightmap page current for rendering.  You only have to 
	// do this for materials that require lightmaps.
	virtual void				BindLightmapPage(int lightmapPageID) = 0;

	// inputs are between 0 and 1
	virtual void				DepthRange(float zNear, float zFar) = 0;

	virtual void				ClearBuffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false) = 0;

	// read to a unsigned char rgb image.
	virtual void				ReadPixels(int x, int y, int width, int height, unsigned char *data, ImageFormat dstFormat) = 0;
	virtual void				ReadPixelsAsync(int, int, int, int, unsigned char *, ImageFormat, ITexture *,/* CThreadEvent * */ void*) = 0;
	virtual void				ReadPixelsAsyncGetResult(int, int, int, int, unsigned char *, ImageFormat,/* CThreadEvent * */ void*) = 0;
	// Sets lighting
	virtual void				SetLightingState(MaterialLightingState_t const&) = 0;
	//virtual void				SetAmbientLight(float r, float g, float b) = 0;
	virtual void				SetLights(int lightNum, const LightDesc_t& desc) = 0;

	// The faces of the cube are specified in the same order as cubemap textures
	virtual void				SetAmbientLightCube(Vector4D cube[6]) = 0;

	// Blit the backbuffer to the framebuffer texture
	virtual void				CopyRenderTargetToTexture(ITexture *pTexture) = 0;

	// Set the current texture that is a copy of the framebuffer.
	virtual void				SetFrameBufferCopyTexture(ITexture *pTexture, int textureIndex = 0) = 0;
	virtual ITexture		   *GetFrameBufferCopyTexture(int textureIndex) = 0;

	//
	// end vertex array api
	//

	// matrix api
	virtual void				MatrixMode(MaterialMatrixMode_t matrixMode) = 0;
	virtual void				PushMatrix() = 0;
	virtual void				PopMatrix() = 0;
	virtual void				LoadMatrix(matrix3x4_t const& matrix) = 0;
	virtual void				LoadMatrix(VMatrix const& matrix) = 0;
	virtual void				MultMatrix(matrix3x4_t const& matrix) = 0;
	virtual void				MultMatrix(VMatrix const& matrix) = 0;
	virtual void				MultMatrixLocal(matrix3x4_t const& matrix) = 0;
	virtual void				MultMatrixLocal(VMatrix const& matrix) = 0;
	virtual void				GetMatrix(MaterialMatrixMode_t matrixMode, matrix3x4_t *matrix) = 0;
	virtual void				GetMatrix(MaterialMatrixMode_t matrixMode, VMatrix *matrix) = 0;
	virtual void				LoadIdentity(void) = 0;
	virtual void				Ortho(double left, double top, double right, double bottom, double zNear, double zFar) = 0;
	virtual void				PerspectiveX(double fovx, double aspect, double zNear, double zFar) = 0;
	virtual void				PickMatrix(int x, int y, int width, int height) = 0;
	virtual void				Rotate(float angle, float x, float y, float z) = 0;
	virtual void				Translate(float x, float y, float z) = 0;
	virtual void				Scale(float x, float y, float z) = 0;
	// end matrix api

	// Sets/gets the viewport
	virtual void				Viewport(int x, int y, int width, int height) = 0;
	virtual void				GetViewport(int& x, int& y, int& width, int& height) const = 0;

	// The cull mode
	virtual void				CullMode(MaterialCullMode_t cullMode) = 0;
	virtual void				FlipCullMode() = 0;
	// end matrix api

	virtual void				BeginGeneratingCSMs() = 0;
	virtual void				EndGeneratingCSMs() = 0;
	virtual void				PerpareForCascadeDraw(int, float, float) = 0;

	// This could easily be extended to a general user clip plane
	virtual void				SetHeightClipMode(MaterialHeightClipMode_t nHeightClipMode) = 0;
	// garymcthack : fog z is always used for heightclipz for now.
	virtual void				SetHeightClipZ(float z) = 0;

	// Fog methods...
	virtual void				FogMode(MaterialFogMode_t fogMode) = 0;
	virtual void				FogStart(float fStart) = 0;
	virtual void				FogEnd(float fEnd) = 0;
	virtual void				SetFogZ(float fogZ) = 0;
	virtual MaterialFogMode_t	GetFogMode(void) = 0;

	virtual void				FogColor3f(float r, float g, float b) = 0;
	virtual void				FogColor3fv(float const* rgb) = 0;
	virtual void				FogColor3ub(unsigned char r, unsigned char g, unsigned char b) = 0;
	virtual void				FogColor3ubv(unsigned char const* rgb) = 0;

	virtual void				GetFogColor(unsigned char *rgb) = 0;

	// Sets the number of bones for skinning
	virtual void				SetNumBoneWeights(int numBones) = 0;

	// Creates/destroys Mesh
	virtual IMesh* CreateStaticMesh(VertexFormat_t fmt, const char *pTextureBudgetGroup, IMaterial * pMaterial = nullptr) = 0;
	virtual void DestroyStaticMesh(IMesh* mesh) = 0;

	// Gets the dynamic mesh associated with the currently bound material
	// note that you've got to render the mesh before calling this function 
	// a second time. Clients should *not* call DestroyStaticMesh on the mesh 
	// returned by this call.
	// Use buffered = false if you want to not have the mesh be buffered,
	// but use it instead in the following pattern:
	//		meshBuilder.Begin
	//		meshBuilder.End
	//		Draw partial
	//		Draw partial
	//		Draw partial
	//		meshBuilder.Begin
	//		meshBuilder.End
	//		etc
	// Use Vertex or Index Override to supply a static vertex or index buffer
	// to use in place of the dynamic buffers.
	//
	// If you pass in a material in pAutoBind, it will automatically bind the
	// material. This can be helpful since you must bind the material you're
	// going to use BEFORE calling GetDynamicMesh.
	virtual IMesh* GetDynamicMesh(
		bool buffered = true,
		IMesh* pVertexOverride = nullptr,
		IMesh* pIndexOverride = nullptr,
		IMaterial *pAutoBind = nullptr) = 0;

	// ------------ New Vertex/Index Buffer interface ----------------------------
	// Do we need support for bForceTempMesh and bSoftwareVertexShader?
	// I don't think we use bSoftwareVertexShader anymore. .need to look into bForceTempMesh.
	virtual IVertexBuffer *CreateStaticVertexBuffer(VertexFormat_t fmt, int nVertexCount, const char *pTextureBudgetGroup) = 0;
	virtual IIndexBuffer *CreateStaticIndexBuffer(MaterialIndexFormat_t fmt, int nIndexCount, const char *pTextureBudgetGroup) = 0;
	virtual void DestroyVertexBuffer(IVertexBuffer *) = 0;
	virtual void DestroyIndexBuffer(IIndexBuffer *) = 0;
	// Do we need to specify the stream here in the case of locking multiple dynamic VBs on different streams?
	virtual IVertexBuffer *GetDynamicVertexBuffer(int streamID, VertexFormat_t vertexFormat, bool bBuffered = true) = 0;
	virtual IIndexBuffer *GetDynamicIndexBuffer(MaterialIndexFormat_t fmt, bool bBuffered = true) = 0;
	virtual void BindVertexBuffer(int streamID, IVertexBuffer *pVertexBuffer, int nOffsetInBytes, int nFirstVertex, int nVertexCount, VertexFormat_t fmt, int nRepetitions = 1) = 0;
	virtual void BindIndexBuffer(IIndexBuffer *pIndexBuffer, int nOffsetInBytes) = 0;
	virtual void Draw(MaterialPrimitiveType_t primitiveType, int firstIndex, int numIndices) = 0;
	// ------------ End ----------------------------

	// Selection mode methods
	virtual int  SelectionMode(bool selectionMode) = 0;
	virtual void SelectionBuffer(unsigned int* pBuffer, int size) = 0;
	virtual void ClearSelectionNames() = 0;
	virtual void LoadSelectionName(int name) = 0;
	virtual void PushSelectionName(int name) = 0;
	virtual void PopSelectionName() = 0;

	// Sets the Clear Color for ClearBuffer....
	virtual void		ClearColor3ub(unsigned char r, unsigned char g, unsigned char b) = 0;
	virtual void		ClearColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;

	// Allows us to override the depth buffer setting of a material
	virtual void	OverrideDepthEnable(bool bEnable, bool bDepthEnable, bool unknown) = 0;

	// FIXME: This is a hack required for NVidia/XBox, can they fix in drivers?
	virtual void	DrawScreenSpaceQuad(IMaterial* pMaterial) = 0;

	// For debugging and building recording files. This will stuff a token into the recording file,
	// then someone doing a playback can watch for the token.
	virtual void	SyncToken(const char *pToken) = 0;

	// FIXME: REMOVE THIS FUNCTION!
	// The only reason why it's not gone is because we're a week from ship when I found the bug in it
	// and everything's tuned to use it.
	// It's returning values which are 2x too big (it's returning sphere diameter x2)
	// Use ComputePixelDiameterOfSphere below in all new code instead.
	virtual float	ComputePixelWidthOfSphere(const Vector& origin, float flRadius) = 0;

	//
	// Occlusion query support
	//

	// Allocate and delete query objects.
	virtual OcclusionQueryObjectHandle_t CreateOcclusionQueryObject(void) = 0;
	virtual void DestroyOcclusionQueryObject(OcclusionQueryObjectHandle_t) = 0;

	// Bracket drawing with begin and end so that we can get counts next frame.
	virtual void BeginOcclusionQueryDrawing(OcclusionQueryObjectHandle_t) = 0;
	virtual void EndOcclusionQueryDrawing(OcclusionQueryObjectHandle_t) = 0;

	// Get the number of pixels rendered between begin and end on an earlier frame.
	// Calling this in the same frame is a huge perf hit!
	virtual int OcclusionQuery_GetNumPixelsRendered(OcclusionQueryObjectHandle_t) = 0;

	virtual void SetFlashlightMode(bool bEnable) = 0;

	virtual void SetFlashlightState(const FlashlightState_t &state, const VMatrix &worldToTexture) = 0;

	virtual bool IsCascadedShadowMapping() = 0;
	virtual void SetCascadedShadowMapping(bool) = 0;
	virtual void SetCascadedShadowMappingState(/*CascadedShadowMappingState_t*/ int const&, ITexture *) = 0;

	// Gets the current height clip mode
	virtual MaterialHeightClipMode_t GetHeightClipMode() = 0;

	// This returns the diameter of the sphere in pixels based on 
	// the current model, view, + projection matrices and viewport.
	virtual float	ComputePixelDiameterOfSphere(const Vector& vecAbsOrigin, float flRadius) = 0;

	// By default, the material system applies the VIEW and PROJECTION matrices	to the user clip
	// planes (which are specified in world space) to generate projection-space user clip planes
	// Occasionally (for the particle system in hl2, for example), we want to override that
	// behavior and explictly specify a ViewProj transform for user clip planes
	virtual void	EnableUserClipTransformOverride(bool bEnable) = 0;
	virtual void	UserClipTransform(const VMatrix &worldToView) = 0;

	virtual bool GetFlashlightMode() const = 0;

	virtual bool IsCullingEnabledForSinglePassFlashlight() = 0;
	virtual void EnableCullingForSinglePassFlashlight(bool) = 0;
	// Used to make the handle think it's never had a successful query before
	virtual void ResetOcclusionQueryObject(OcclusionQueryObjectHandle_t) = 0;

	// Creates/destroys morph data associated w/ a particular material
	virtual IMorph *CreateMorph(MorphFormat_t format, const char *pDebugName) = 0;
	virtual void DestroyMorph(IMorph *pMorph) = 0;

	// Binds the morph data for use in rendering
	virtual void BindMorph(IMorph *pMorph) = 0;

	// Sets flexweights for rendering
	virtual void SetFlexWeights(int nFirstWeight, int nCount, const MorphWeight_t* pWeights) = 0;

	virtual void LockRenderData(int) = 0;
	virtual void UnlockRenderData() = 0;
	virtual void AddRefRenderData() = 0;
	virtual void ReleaseRenderData() = 0;
	virtual void IsRenderData(void const*) = 0;

	// Read w/ stretch to a host-memory buffer
	virtual void ReadPixelsAndStretch(Rect_t *pSrcRect, Rect_t *pDstRect, unsigned char *pBuffer, ImageFormat dstFormat, int nDstStride) = 0;

	// Gets the window size
	virtual void GetWindowSize(int &width, int &height) const = 0;

	// This function performs a texture map from one texture map to the render destination, doing
	// all the necessary pixel/texel coordinate fix ups. fractional values can be used for the
	// src_texture coordinates to get linear sampling - integer values should produce 1:1 mappings
	// for non-scaled operations.
	virtual void DrawScreenSpaceRectangle(
		IMaterial *pMaterial,
		int destx, int desty,
		int width, int height,
		float src_texture_x0, float src_texture_y0,			// which texel you want to appear at
															// destx/y
		float src_texture_x1, float src_texture_y1,			// which texel you want to appear at
															// destx+width-1, desty+height-1
		int src_texture_width, int src_texture_height,		// needed for fixup
		void *pClientRenderable = nullptr,
		int nXDice = 1,
		int nYDice = 1) = 0;

	virtual void LoadBoneMatrix(int boneIndex, const matrix3x4_t& matrix) = 0;

	// This version will push the current rendertarget + current viewport onto the stack
	virtual void PushRenderTargetAndViewport() = 0;

	// This version will push a new rendertarget + a maximal viewport for that rendertarget onto the stack
	virtual void PushRenderTargetAndViewport(ITexture *pTexture) = 0;

	// This version will push a new rendertarget + a specified viewport onto the stack
	virtual void PushRenderTargetAndViewport(ITexture *pTexture, int nViewX, int nViewY, int nViewW, int nViewH) = 0;

	// This version will push a new rendertarget + a specified viewport onto the stack
	virtual void PushRenderTargetAndViewport(ITexture *pTexture, ITexture *pDepthTexture, int nViewX, int nViewY, int nViewW, int nViewH) = 0;

	// This will pop a rendertarget + viewport
	virtual void PopRenderTargetAndViewport() = 0;

	// Binds a particular texture as the current lightmap
	virtual void BindLightmapTexture(ITexture *pLightmapTexture) = 0;

	// Blit a subrect of the current render target to another texture
	virtual void CopyRenderTargetToTextureEx(ITexture *pTexture, int nRenderTargetID, Rect_t *pSrcRect, Rect_t *pDstRect = nullptr) = 0;
	virtual void CopyRenderTargetToTextureExCopyTextureToRenderTargetEx(int, ITexture *, Rect_t *, Rect_t *) = 0;
	// Special off-center perspective matrix for DoF, MSAA jitter and poster rendering
	virtual void PerspectiveOffCenterX(double fovx, double aspect, double zNear, double zFar, double bottom, double top, double left, double right) = 0;

	// Rendering parameters control special drawing modes withing the material system, shader
	// system, shaders, and engine. renderparm.h has their definitions.
	virtual void SetFloatRenderingParameter(int parm_number, float value) = 0;
	virtual void SetIntRenderingParameter(int parm_number, int value) = 0;
	virtual void SetVectorRenderingParameter(int parm_number, Vector const &value) = 0;

	// stencil buffer operations.
	virtual void SetStencilState(ShaderStencilState_t const&) = 0;
	//virtual void SetStencilEnable(bool onoff) = 0;
	//virtual void SetStencilFailOperation(StencilOperation_t op) = 0;
	//virtual void SetStencilZFailOperation(StencilOperation_t op) = 0;
	//virtual void SetStencilPassOperation(StencilOperation_t op) = 0;
	//virtual void SetStencilCompareFunction(StencilComparisonFunction_t cmpfn) = 0;
	//virtual void SetStencilReferenceValue(int ref) = 0;
	//virtual void SetStencilTestMask(uint32 msk) = 0;
	//virtual void SetStencilWriteMask(uint32 msk) = 0;
	virtual void ClearStencilBufferRectangle(int xmin, int ymin, int xmax, int ymax, int value) = 0;

	virtual void SetRenderTargetEx(int nRenderTargetID, ITexture *pTexture) = 0;

	// rendering clip planes, beware that only the most recently pushed plane will actually be used in a sizeable chunk of hardware configurations
	// and that changes to the clip planes mid-frame while UsingFastClipping() is true will result unresolvable depth inconsistencies
	virtual void PushCustomClipPlane(const float *pPlane) = 0;
	virtual void PopCustomClipPlane() = 0;

	// Returns the number of vertices + indices we can render using the dynamic mesh
	// Passing true in the second parameter will return the max # of vertices + indices
	// we can use before a flush is provoked and may return different values 
	// if called multiple times in succession. 
	// Passing false into the second parameter will return
	// the maximum possible vertices + indices that can be rendered in a single batch
	virtual void GetMaxToRender(IMesh *pMesh, bool bMaxUntilFlush, int *pMaxVerts, int *pMaxIndices) = 0;

	// Returns the max possible vertices + indices to render in a single draw call
	virtual int GetMaxVerticesToRender(IMaterial *pMaterial) = 0;
	virtual int GetMaxIndicesToRender() = 0;
	virtual void DisableAllLocalLights() = 0;
	virtual int CompareMaterialCombos(IMaterial *pMaterial1, IMaterial *pMaterial2, int lightMapID1, int lightMapID2) = 0;

	virtual IMesh *GetFlexMesh() = 0;

	virtual void SetFlashlightStateEx(const FlashlightState_t &state, const VMatrix &worldToTexture, ITexture *pFlashlightDepthTexture) = 0;

	// Returns the currently bound local cubemap
	virtual ITexture *GetLocalCubemap() = 0;

	// This is a version of clear buffers which will only clear the buffer at pixels which pass the stencil test
	virtual void ClearBuffersObeyStencil(bool bClearColor, bool bClearDepth) = 0;

	//enables/disables all entered clipping planes, returns the input from the last time it was called.
	virtual bool EnableClipping(bool bEnable) = 0;

	//get fog distances entered with FogStart(), FogEnd(), and SetFogZ()
	virtual void GetFogDistances(float *fStart, float *fEnd, float *fFogZ) = 0;

	// Hooks for firing PIX events from outside the Material System...
	virtual void BeginPIXEvent(unsigned long color, const char *szName) = 0;
	virtual void EndPIXEvent() = 0;
	virtual void SetPIXMarker(unsigned long color, const char *szName) = 0;

	// Batch API
	// from changelist 166623:
	// - replaced obtuse material system batch usage with an explicit and easier to thread API
	virtual void BeginBatch(IMesh* pIndices) = 0;
	virtual void BindBatch(IMesh* pVertices, IMaterial *pAutoBind = nullptr) = 0;
	virtual void DrawBatch(int firstIndex, int numIndices) = 0;
	virtual void EndBatch() = 0;

	// Raw access to the call queue, which can be NULL if not in a queued mode
	virtual ICallQueue *GetCallQueue() = 0;

	// Returns the world-space camera position
	virtual void GetWorldSpaceCameraPosition(Vector *pCameraPos) = 0;
	virtual void GetWorldSpaceCameraVectors(Vector *pVecForward, Vector *pVecRight, Vector *pVecUp) = 0;

	// Tone mapping
	//virtual void				ResetToneMappingScale(float monoscale) = 0; 			// set scale to monoscale instantly with no chasing
	//virtual void				SetGoalToneMappingScale(float monoscale) = 0; 			// set scale to monoscale instantly with no chasing

	// call TurnOnToneMapping before drawing the 3d scene to get the proper interpolated brightness
	// value set.
	//virtual void				TurnOnToneMapping() = 0;

	// Set a linear vector color scale for all 3D rendering.
	// A value of [1.0f, 1.0f, 1.0f] should match non-tone-mapped rendering.
	virtual void				SetToneMappingScaleLinear(const Vector &scale) = 0;

	virtual Vector				GetToneMappingScaleLinear() = 0;
	virtual void				SetShadowDepthBiasFactors(float fSlopeScaleDepthBias, float fDepthBias) = 0;

	// Apply stencil operations to every pixel on the screen without disturbing depth or color buffers
	virtual void				PerformFullScreenStencilOperation(void) = 0;
};

class IModelRender
{
public:
	virtual int                     DrawModel(int flags, IClientRenderable *pRenderable, ModelInstanceHandle_t instance, int entity_index, const model_t *model, Vector const& origin, QAngle const& angles, int skin, int body, int hitboxset, const matrix3x4_t *modelToWorld = NULL, const matrix3x4_t *pLightingOffset = NULL) = 0;
	virtual void                    ForcedMaterialOverride(IMaterial *newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL, int nOverrides = 0) = 0;
	virtual bool                    IsForcedMaterialOverride(void) = 0;
};