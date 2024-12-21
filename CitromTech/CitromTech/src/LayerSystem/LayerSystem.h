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

		virtual void OnUpdate() {}

		virtual void OnTick() {}
		virtual void OnRender() {}
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

		void Update()
		{
			for (Layer* layer : m_Layers)
				layer->OnUpdate();
		}
		void Tick()
		{
			for (Layer* layer : m_Layers)
				layer->OnTick();
		}
		void Render()
		{
			for (Layer* layer : m_Layers)
				layer->OnRender();
		}
	private:
		CTL::DArray<Layer*> m_Layers;
	};
}