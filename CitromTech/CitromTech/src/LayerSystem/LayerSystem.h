#pragma once

#include "Core.h"

#include "CTL/DArray.h"

namespace Citrom
{
	class Layer
	{
	public:
		Layer() {}
		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(float64 deltaTime) {}

		virtual void OnTick(float64 fixedDeltaTime) {}
		virtual void OnRender() {}

		virtual void OnImGuiRender() {}
	};
	
	class LayerStack
	{
	public:
		void Push(Layer* layer)
		{
			m_Layers.PushBack(layer);
			layer->OnAttach();
		}
		void Pop()
		{
			m_Layers[m_Layers.Count() - 1]->OnDetach();
			m_Layers.PopBack();
		}

		void Update(float64 deltaTime)
		{
			for (Layer* layer : m_Layers)
				layer->OnUpdate(deltaTime);
		}
		void Tick(float64 fixedDeltaTime)
		{
			for (Layer* layer : m_Layers)
				layer->OnTick(fixedDeltaTime);
		}
		void Render()
		{
			for (Layer* layer : m_Layers)
				layer->OnRender();
		}
		void ImGuiRender()
		{
			for (Layer* layer : m_Layers)
				layer->OnImGuiRender();
		}
	private:
		CTL::DArray<Layer*> m_Layers;
	};
}