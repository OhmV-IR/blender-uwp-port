#pragma once
#include "Common/DeviceResources.h"
#include "Common/DirectXHelper.h"
#include "d2d1.h"
#include <wrl/client.h>

using namespace Microsoft::WRL;
	class TextDisplay
	{
	public:
		TextDisplay(const std::shared_ptr<DX::DeviceResources>& deviceResources, std::wstring text, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlign, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STRETCH fontStretch, float fontSize, D2D1_COLOR_F brushColor, DWRITE_TEXT_ALIGNMENT textAlign, float offsetLeft, float offsetTop, float textWidth, float textHeight);
		void Update();
		void Render();
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();
	private:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::wstring m_text;
		DWRITE_PARAGRAPH_ALIGNMENT m_paragraphAlign;
		DWRITE_TEXT_ALIGNMENT m_textAlign;
		DWRITE_FONT_WEIGHT m_fontWeight;
		DWRITE_FONT_STYLE m_fontStyle;
		DWRITE_FONT_STRETCH m_fontStretch;
		float m_fontSize;
		float m_offsetLeft;
		float m_offsetTop;
		float m_textWidth;
		float m_textHeight;
		D2D1_COLOR_F m_brushColor;
		DWRITE_TEXT_METRICS m_textMetrics;
		ComPtr<ID2D1SolidColorBrush> m_textBrush;
		ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;
		ComPtr<IDWriteTextLayout3> m_textLayout;
		ComPtr<IDWriteTextFormat2> m_textFormat;
	};