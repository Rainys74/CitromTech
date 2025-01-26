#include "DX11Device.h"
#include "Renderer/RenderAPI/Texture.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

namespace Citrom::RenderAPI
{
	struct Texture2DDX11
	{
	};

	Texture2D DX11Device::CreateTexture2D(Texture2DDesc* descriptor)
	{
		CREATE_BUFFER_INTERNAL(Texture2D, Texture2DDX11, tex2D, internalData);

		return tex2D;
	}
	void DX11Device::BindTexture2D(Texture2D* tex2D)
	{
		GET_BUFFER_INTERNAL(Texture2DDX11, tex2D, internalData);
	}
}
#endif