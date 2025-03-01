#pragma once

#include "LayerSystem/LayerSystem.h"

class EditorLayer : public Citrom::Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(float64 deltaTime) override;
	void OnImGuiRender() override;
};

constexpr bool IsEditorBuild()
{
#ifdef CT_EDITOR_ENABLED
	return true;
#else
	return false;
#endif
}

#ifdef CT_EDITOR_ENABLED
#define IF_EDITOR(x) x
#define IF_NEDITOR(x)
#else
#define IF_EDITOR(x)
#define IF_NEDITOR(x) x
#endif