#pragma once

#include "Graphics.h"
#include "CTL/RefPointer.h"
#include "CTL/DArray.h"

namespace Citrom::RenderAPI
{
	// TODO: maybe i should just combine FBFormat and Format into one?
	enum class FramebufferFormat : uint8fast
	{
		None,

		RGBA8,
		D16N, // U2F
		D32F,
		DEPTH24STENCIL8, // U2F/UINT

		// defaults
		Color = RGBA8,
		Depth = D32F,
		DepthStencil = DEPTH24STENCIL8
	};

	enum class FramebufferAttachmentType : uint8fast
	{
		Color,
		DepthStencil
	};

	struct FramebufferAttachment
	{
		FramebufferAttachmentType type;
		FramebufferFormat format;
	};

	struct FramebufferAttachments
	{
		FramebufferAttachments() = default;
		FramebufferAttachments(std::initializer_list<FramebufferAttachment> initList)
		{
			attachments.Reserve(initList.size());
			for (const auto& attachment : initList)
			{
				attachments.PushBack(attachment);
			}
		}
		FramebufferAttachments& operator=(std::initializer_list<FramebufferAttachment> initList)
		{
			attachments.Clear();
			attachments.Resize(initList.size()); //Reserve(initList.size());
			for (const FramebufferAttachment& attachment : initList)
			{
				attachments.PushBack(attachment);
			}
			return *this;
		}

		void PushAttachment(FramebufferAttachmentType type, FramebufferFormat format)
		{
			attachments.PushBack(FramebufferAttachment{type, format});
		}

		bool HasAttachmentType(FramebufferAttachmentType type) const
		{
			for (const auto& attachment : attachments)
			{
				if (attachment.type == type)
					return true;
			}
			return false;
		}
		FramebufferFormat GetDepthAttachmentFormat()
		{
			for (const auto& attachment : attachments)
			{
				if (attachment.type == FramebufferAttachmentType::DepthStencil)
					return attachment.format;
			}
			return FramebufferFormat::None;
		}

		CTL::DArray<FramebufferAttachment> attachments;
	};

	struct FramebufferDesc
	{
		uint32 width = 0, height = 0; // 0 will take the screen's size
		uint32 samples = 1;

		FramebufferAttachments* attachments;
	};

	struct Framebuffer
	{
		FramebufferDesc descriptor;
		CTL::Ref<void> internal;
	};

	// Render Pass
	enum class RenderPassLoadOp
	{
		Clear,
		Load,
		NotNeeded // used for render passes you do not care about what happens to their displays, avoids the ambiguity of DontCare, also valid names: Discard, Unused, Unneeded, Any
	};

	struct RenderPassDesc
	{
		Framebuffer* targetFramebuffer;
		struct ClearColorValues
		{
			float32 clearColor[4];

			ClearColorValues() = default;
			ClearColorValues(std::initializer_list<float> list)
			{
				std::copy(list.begin(), list.end(), clearColor);
			}

			FORCE_INLINE float32& operator[](size_t index) { return *(float32*)(&clearColor[0] + index); }
			FORCE_INLINE const float32& operator[](size_t index) const { return *(float32*)(&clearColor[0] + index); }
		} clearColor;
		RenderPassLoadOp loadOp = RenderPassLoadOp::Clear;
	};
	struct RenderPass
	{
		RenderPassDesc descriptor;
		CTL::Ref<void> internal;
	};
}