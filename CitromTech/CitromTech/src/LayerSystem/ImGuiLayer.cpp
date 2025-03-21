#include "ImGuiLayer.h"
#include "ImGui/ImGuiStyles.h"

#include "Platform/PlatformImGui.h"
#include "Renderer/Renderer.h"

#include "Profiling/Profiler.h"

#include "imgui.h"
#include "ImGuizmo.h"

// ImGui Notify by TyomaVader (Original by patrickcjk)
#include "Vendor/ImGuiNotify/ImGuiNotify.hpp"
#include "Vendor/ImGuiNotify/IconsFontAwesome6.h"
#include "Vendor/ImGuiNotify/fa-solid-900.h"

// ImCandy
#define M_PI PI_MACRO(0)
#include "Vendor/ImCandy/candy.h"

namespace Citrom
{
	static float g_ResizeFont = 0.0f;

	ImGuiLayer::ImGuiLayer()
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}

	/*
	void ImGuiLayer::RebuildFontsHint(float scale)
	{
		/*ImGuiIO& io = ImGui::GetIO();

		io.Fonts->Clear();

		constexpr float fontSize = 17.0f;

		io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Roboto/Roboto-Bold.ttf", fontSize * scale); // Black
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Roboto/Roboto-Regular.ttf", fontSize * scale);* /

		constexpr float fontSize = 17.0f;

		g_ResizeFont = fontSize * scale;
	}
	*/

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		constexpr float fontSize = 17.0f; //13.0f;

		// Roboto
		io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Roboto/Roboto-Bold.ttf", fontSize); // Black
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Roboto/Roboto-Regular.ttf", fontSize);
		// Inter
		//io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Inter/Inter_24pt-Bold.ttf", fontSize); // Black
		//io.FontDefault = io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Inter/Inter_24pt-Regular.ttf", fontSize);
		// Noto Sans CJK SC
		//io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Inter/Inter_24pt-Bold.ttf", fontSize); // Black
		//io.FontDefault = io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/NotoSansCJKSC/NotoSansCJKsc-Regular.ttf", fontSize * 1.3f);

		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		//ImGui::StyleColorsLight();

		ImGuiStyles::Push(ImGuiStyleObj{"Dark Ruda",			ImThemes::DarkRuda});
		ImGuiStyles::Push(ImGuiStyleObj{"Comfy",				ImThemes::Comfy});
		ImGuiStyles::Push(ImGuiStyleObj{"Cherry",				ImThemes::Cherry});
		ImGuiStyles::Push(ImGuiStyleObj{"Quick Minimal Look",	ImThemes::QuickMinimalLook});

		ImGuiStyles::Push(ImGuiStyleObj{"ImCandy Blender",	[]() {ImCandy::Theme_Blender();}	});
		ImGuiStyles::Push(ImGuiStyleObj{"ImCandy Nord",		[]() {ImCandy::Theme_Nord();}		});

		ImGuiStyles::Push(ImGuiStyleObj{"ImGui Dark",		[]() {ImGui::StyleColorsDark();}	});
		ImGuiStyles::Push(ImGuiStyleObj{"ImGui Classic",	[]() {ImGui::StyleColorsClassic();} });
		ImGuiStyles::Push(ImGuiStyleObj{"ImGui Light",		[]() {ImGui::StyleColorsLight();}	});

		// Select Default Style/Theme
		ImGuiStyles::GetStyles()[0].callback();
	}
	void ImGuiLayer::OnDetach()
	{
		CT_PROFILE_MEMBER_FUNCTION();

		Platform::ImGui::Terminate();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Initialize(Platform::Window* window)
	{
		Platform::ImGui::Initialize(window);
		Renderer::ImGuiInit();

		// ImGui Notify
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontDefault();

		static constexpr float baseFontSize = 16.0f;
		static constexpr float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

		static constexpr ImWchar iconsRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
		ImFontConfig iconsConfig;
		iconsConfig.MergeMode = true;
		iconsConfig.PixelSnapH = true;
		iconsConfig.GlyphMinAdvanceX = iconFontSize;
		io.Fonts->AddFontFromMemoryCompressedTTF(fa_solid_900_compressed_data, fa_solid_900_compressed_size, iconFontSize, &iconsConfig, iconsRanges);
	}

	void ImGuiLayer::Begin()
	{
		Renderer::ImGuiNewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}
	void ImGuiLayer::End()
	{
		CT_PROFILE_MEMBER_FUNCTION();

#if 0 // TyomaVader
		{
			// Notifications style setup
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f); // Disable round borders
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f); // Disable borders

			// Notifications color setup
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 1.00f)); // Background color

			// Main rendering function
			ImGui::RenderNotifications();

			//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧� WARNING 覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�
			// Argument MUST match the amount of ImGui::PushStyleVar() calls 
			ImGui::PopStyleVar(2);
			// Argument MUST match the amount of ImGui::PushStyleColor() calls 
			ImGui::PopStyleColor(1);
		}
#else // patrickcjk
		{
			// Render toasts on top of everything, at the end of your code!
			// You should push style vars here
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f)); // Background color

			ImGui::RenderNotifications(); // <-- Here we render all notifications
			ImGui::PopStyleVar(1); // Don't forget to Pop()
			ImGui::PopStyleColor(1);
		}
#endif

		ImGui::Render();
		Renderer::ImGuiRenderDrawData(ImGui::GetDrawData());

		/*
		// Resize Fonts
		if (g_ResizeFont != 0.0f)
		{
			ImGuiIO& io = ImGui::GetIO();

			io.Fonts->Clear();

			io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Roboto/Roboto-Bold.ttf", g_ResizeFont); // Black
			io.FontDefault = io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Roboto/Roboto-Regular.ttf", g_ResizeFont);
			//CT_WARN("G_RESIZE_FONT! {}", g_ResizeFont);

			// Required for dynamic font loading
			//	io.Fonts->GetTexDataAsRGBA32();
			io.Fonts->Build();

			g_ResizeFont = 0.0f;
		}
		*/
	}
	namespace ImThemes
	{
		void DarkRuda()
		{
			// Dark Ruda style by Raikiri from ImThemes
			ImGuiStyle& style = ImGui::GetStyle();

			style.Alpha = 1.0f;
			style.DisabledAlpha = 0.6000000238418579f;
			style.WindowPadding = ImVec2(8.0f, 8.0f);
			style.WindowRounding = 0.0f;
			style.WindowBorderSize = 1.0f;
			style.WindowMinSize = ImVec2(32.0f, 32.0f);
			style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
			style.WindowMenuButtonPosition = ImGuiDir_Left;
			style.ChildRounding = 0.0f;
			style.ChildBorderSize = 1.0f;
			style.PopupRounding = 0.0f;
			style.PopupBorderSize = 1.0f;
			style.FramePadding = ImVec2(4.0f, 3.0f);
			style.FrameRounding = 4.0f;
			style.FrameBorderSize = 0.0f;
			style.ItemSpacing = ImVec2(8.0f, 4.0f);
			style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
			style.CellPadding = ImVec2(4.0f, 2.0f);
			style.IndentSpacing = 21.0f;
			style.ColumnsMinSpacing = 6.0f;
			style.ScrollbarSize = 14.0f;
			style.ScrollbarRounding = 9.0f;
			style.GrabMinSize = 10.0f;
			style.GrabRounding = 4.0f;
			style.TabRounding = 4.0f;
			style.TabBorderSize = 0.0f;
			style.TabMinWidthForCloseButton = 0.0f;
			style.ColorButtonPosition = ImGuiDir_Right;
			style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
			style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

			style.Colors[ImGuiCol_Text] = ImVec4(0.9490196108818054f, 0.95686274766922f, 0.9764705896377563f, 1.0f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.3568627536296844f, 0.4196078479290009f, 0.4666666686534882f, 1.0f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1098039224743843f, 0.1490196138620377f, 0.168627455830574f, 1.0f);
			style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1490196138620377f, 0.1764705926179886f, 0.2196078449487686f, 1.0f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.0784313753247261f, 0.09803921729326248f, 0.1176470592617989f, 1.0f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2000000029802322f, 0.2470588237047195f, 0.2862745225429535f, 1.0f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1176470592617989f, 0.2000000029802322f, 0.2784313857555389f, 1.0f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.08627451211214066f, 0.1176470592617989f, 0.1372549086809158f, 1.0f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08627451211214066f, 0.1176470592617989f, 0.1372549086809158f, 0.6499999761581421f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0784313753247261f, 0.09803921729326248f, 0.1176470592617989f, 1.0f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1490196138620377f, 0.1764705926179886f, 0.2196078449487686f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.3899999856948853f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.2000000029802322f, 0.2470588237047195f, 0.2862745225429535f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1764705926179886f, 0.2196078449487686f, 0.2470588237047195f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.08627451211214066f, 0.2078431397676468f, 0.3098039329051971f, 1.0f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.2784313857555389f, 0.5568627715110779f, 1.0f, 1.0f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.2784313857555389f, 0.5568627715110779f, 1.0f, 1.0f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.3686274588108063f, 0.6078431606292725f, 1.0f, 1.0f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.2000000029802322f, 0.2470588237047195f, 0.2862745225429535f, 1.0f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2784313857555389f, 0.5568627715110779f, 1.0f, 1.0f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05882352963089943f, 0.529411792755127f, 0.9764705896377563f, 1.0f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.2000000029802322f, 0.2470588237047195f, 0.2862745225429535f, 0.550000011920929f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
			style.Colors[ImGuiCol_Separator] = ImVec4(0.2000000029802322f, 0.2470588237047195f, 0.2862745225429535f, 1.0f);
			style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 0.7799999713897705f);
			style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.25f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.6700000166893005f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);
			style.Colors[ImGuiCol_Tab] = ImVec4(0.1098039224743843f, 0.1490196138620377f, 0.168627455830574f, 1.0f);
			style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
			style.Colors[ImGuiCol_TabActive] = ImVec4(0.2000000029802322f, 0.2470588237047195f, 0.2862745225429535f, 1.0f);
			style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1098039224743843f, 0.1490196138620377f, 0.168627455830574f, 1.0f);
			style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1098039224743843f, 0.1490196138620377f, 0.168627455830574f, 1.0f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
			style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
			style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
			style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
			style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
			style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
			style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
			style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
			style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
			style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
		}

		void QuickMinimalLook()
		{
			// Quick minimal look style by 90th from ImThemes
			ImGuiStyle& style = ImGui::GetStyle();

			style.Alpha = 1.0f;
			style.DisabledAlpha = 0.300000011920929f;
			style.WindowPadding = ImVec2(6.5f, 2.700000047683716f);
			style.WindowRounding = 0.0f;
			style.WindowBorderSize = 1.0f;
			style.WindowMinSize = ImVec2(20.0f, 32.0f);
			style.WindowTitleAlign = ImVec2(0.0f, 0.6000000238418579f);
			style.WindowMenuButtonPosition = ImGuiDir_None;
			style.ChildRounding = 0.0f;
			style.ChildBorderSize = 1.0f;
			style.PopupRounding = 10.10000038146973f;
			style.PopupBorderSize = 1.0f;
			style.FramePadding = ImVec2(20.0f, 3.5f);
			style.FrameRounding = 0.0f;
			style.FrameBorderSize = 0.0f;
			style.ItemSpacing = ImVec2(4.400000095367432f, 4.0f);
			style.ItemInnerSpacing = ImVec2(4.599999904632568f, 3.599999904632568f);
			style.CellPadding = ImVec2(3.099999904632568f, 6.300000190734863f);
			style.IndentSpacing = 4.400000095367432f;
			style.ColumnsMinSpacing = 5.400000095367432f;
			style.ScrollbarSize = 8.800000190734863f;
			style.ScrollbarRounding = 9.0f;
			style.GrabMinSize = 9.399999618530273f;
			style.GrabRounding = 0.0f;
			style.TabRounding = 0.0f;
			style.TabBorderSize = 0.0f;
			style.TabMinWidthForCloseButton = 0.0f;
			style.ColorButtonPosition = ImGuiDir_Right;
			style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
			style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

			style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05098039284348488f, 0.03529411926865578f, 0.03921568766236305f, 1.0f);
			style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.1019607856869698f, 0.1019607856869698f, 0.1019607856869698f, 0.5f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1607843190431595f, 0.1490196138620377f, 0.1921568661928177f, 1.0f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.5450980663299561f, 0.4666666686534882f, 0.7176470756530762f, 1.0f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3450980484485626f, 0.294117659330368f, 0.4588235318660736f, 1.0f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.3137255012989044f, 0.2588235437870026f, 0.4274509847164154f, 1.0f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.3176470696926117f, 0.2784313857555389f, 0.407843142747879f, 1.0f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4156862795352936f, 0.364705890417099f, 0.529411792755127f, 1.0f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4039215743541718f, 0.3529411852359772f, 0.5098039507865906f, 1.0f);
			style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
			style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_Tab] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3254902064800262f, 0.2862745225429535f, 0.4156862795352936f, 1.0f);
			style.Colors[ImGuiCol_TabActive] = ImVec4(0.4000000059604645f, 0.3490196168422699f, 0.5058823823928833f, 1.0f);
			style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
			style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
			style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
			style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
			style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
			style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
			style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
			style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
			style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
			style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
		}

		void Cherry()
		{
			// Cherry style by r-lyeh from ImThemes
			ImGuiStyle& style = ImGui::GetStyle();

			style.Alpha = 1.0f;
			style.DisabledAlpha = 0.6000000238418579f;
			style.WindowPadding = ImVec2(6.0f, 3.0f);
			style.WindowRounding = 0.0f;
			style.WindowBorderSize = 1.0f;
			style.WindowMinSize = ImVec2(32.0f, 32.0f);
			style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
			style.WindowMenuButtonPosition = ImGuiDir_Left;
			style.ChildRounding = 0.0f;
			style.ChildBorderSize = 1.0f;
			style.PopupRounding = 0.0f;
			style.PopupBorderSize = 1.0f;
			style.FramePadding = ImVec2(5.0f, 1.0f);
			style.FrameRounding = 3.0f;
			style.FrameBorderSize = 1.0f;
			style.ItemSpacing = ImVec2(7.0f, 1.0f);
			style.ItemInnerSpacing = ImVec2(1.0f, 1.0f);
			style.CellPadding = ImVec2(4.0f, 2.0f);
			style.IndentSpacing = 6.0f;
			style.ColumnsMinSpacing = 6.0f;
			style.ScrollbarSize = 13.0f;
			style.ScrollbarRounding = 16.0f;
			style.GrabMinSize = 20.0f;
			style.GrabRounding = 2.0f;
			style.TabRounding = 4.0f;
			style.TabBorderSize = 1.0f;
			style.TabMinWidthForCloseButton = 0.0f;
			style.ColorButtonPosition = ImGuiDir_Right;
			style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
			style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

			style.Colors[ImGuiCol_Text] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.8799999952316284f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.2800000011920929f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 1.0f);
			style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 0.8999999761581421f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.5372549295425415f, 0.47843137383461f, 0.2549019753932953f, 0.1620000004768372f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.7799999713897705f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2313725501298904f, 0.2000000029802322f, 0.2705882489681244f, 1.0f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.501960813999176f, 0.07450980693101883f, 0.2549019753932953f, 1.0f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 0.75f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 0.4699999988079071f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.08627451211214066f, 0.1490196138620377f, 0.1568627506494522f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.7799999713897705f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.7098039388656616f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4666666686534882f, 0.7686274647712708f, 0.8274509906768799f, 0.1400000005960464f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7098039388656616f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.4666666686534882f, 0.7686274647712708f, 0.8274509906768799f, 0.1400000005960464f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.8600000143051147f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.7599999904632568f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.8600000143051147f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.501960813999176f, 0.07450980693101883f, 0.2549019753932953f, 1.0f);
			style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
			style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 0.7799999713897705f);
			style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.4666666686534882f, 0.7686274647712708f, 0.8274509906768799f, 0.03999999910593033f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.7799999713897705f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
			style.Colors[ImGuiCol_Tab] = ImVec4(0.1764705926179886f, 0.3490196168422699f, 0.5764706134796143f, 0.8619999885559082f);
			style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
			style.Colors[ImGuiCol_TabActive] = ImVec4(0.196078434586525f, 0.407843142747879f, 0.6784313917160034f, 1.0f);
			style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 0.9724000096321106f);
			style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.6299999952316284f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.6299999952316284f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
			style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
			style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
			style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
			style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.4300000071525574f);
			style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
			style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
			style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
			style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
			style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
		}

		void Comfy()
		{
			// Comfy style by Giuseppe from ImThemes
			ImGuiStyle& style = ImGui::GetStyle();

			style.Alpha = 1.0f;
			style.DisabledAlpha = 0.1000000014901161f;
			style.WindowPadding = ImVec2(8.0f, 8.0f);
			style.WindowRounding = 10.0f;
			style.WindowBorderSize = 0.0f;
			style.WindowMinSize = ImVec2(30.0f, 30.0f);
			style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
			style.WindowMenuButtonPosition = ImGuiDir_Right;
			style.ChildRounding = 5.0f;
			style.ChildBorderSize = 1.0f;
			style.PopupRounding = 10.0f;
			style.PopupBorderSize = 0.0f;
			style.FramePadding = ImVec2(5.0f, 3.5f);
			style.FrameRounding = 5.0f;
			style.FrameBorderSize = 0.0f;
			style.ItemSpacing = ImVec2(5.0f, 4.0f);
			style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
			style.CellPadding = ImVec2(4.0f, 2.0f);
			style.IndentSpacing = 5.0f;
			style.ColumnsMinSpacing = 5.0f;
			style.ScrollbarSize = 15.0f;
			style.ScrollbarRounding = 9.0f;
			style.GrabMinSize = 15.0f;
			style.GrabRounding = 5.0f;
			style.TabRounding = 5.0f;
			style.TabBorderSize = 0.0f;
			style.TabMinWidthForCloseButton = 0.0f;
			style.ColorButtonPosition = ImGuiDir_Right;
			style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
			style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

			style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(1.0f, 1.0f, 1.0f, 0.3605149984359741f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
			style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.4235294163227081f, 0.3803921639919281f, 0.572549045085907f, 0.54935622215271f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3803921639919281f, 0.4235294163227081f, 0.572549045085907f, 0.5490196347236633f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2588235437870026f, 0.2588235437870026f, 0.2588235437870026f, 0.0f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 0.0f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.2352941185235977f, 0.2352941185235977f, 0.2352941185235977f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.294117659330368f, 0.294117659330368f, 0.294117659330368f, 1.0f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.294117659330368f, 0.294117659330368f, 0.294117659330368f, 1.0f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.8156862854957581f, 0.772549033164978f, 0.9647058844566345f, 0.5490196347236633f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.8156862854957581f, 0.772549033164978f, 0.9647058844566345f, 0.5490196347236633f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.8156862854957581f, 0.772549033164978f, 0.9647058844566345f, 0.5490196347236633f);
			style.Colors[ImGuiCol_Separator] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
			style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
			style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.8156862854957581f, 0.772549033164978f, 0.9647058844566345f, 0.5490196347236633f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.8156862854957581f, 0.772549033164978f, 0.9647058844566345f, 0.5490196347236633f);
			style.Colors[ImGuiCol_Tab] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
			style.Colors[ImGuiCol_TabHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
			style.Colors[ImGuiCol_TabActive] = ImVec4(0.8156862854957581f, 0.772549033164978f, 0.9647058844566345f, 0.5490196347236633f);
			style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.4509803950786591f, 1.0f, 0.0f);
			style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 0.0f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.294117659330368f, 0.294117659330368f, 0.294117659330368f, 1.0f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
			style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
			style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.4235294163227081f, 0.3803921639919281f, 0.572549045085907f, 0.5490196347236633f);
			style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.4235294163227081f, 0.3803921639919281f, 0.572549045085907f, 0.2918455004692078f);
			style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.03433477878570557f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
			style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
			style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
			style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
			style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
			style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
		}
	}
}