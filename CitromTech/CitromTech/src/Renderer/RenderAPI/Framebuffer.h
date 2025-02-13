#pragma once

#include "Graphics.h"
#include "CTL/RefPointer.h"
#include "CTL/DArray.h"

namespace Citrom::RenderAPI
{
	enum class FramebufferFormat : uint8fast
	{
		None,

		RGBA8,
		DEPTH24STENCIL8,
		D32F,

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

	struct RenderPassDesc
	{
		Framebuffer* targetFramebuffer;
	};
	struct RenderPass
	{
		RenderPassDesc descriptor;
		CTL::Ref<void> internal;
	};
}