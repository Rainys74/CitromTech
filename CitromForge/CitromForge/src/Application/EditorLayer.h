#pragma once

#include "LayerSystem/LayerSystem.h"

class EditorLayer : public Citrom::Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate() override;
	void OnImGuiRender() override;
};