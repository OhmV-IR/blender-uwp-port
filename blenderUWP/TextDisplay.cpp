#include "pch.h"
#include "TextDisplay.h"
#include <wrl/client.h>
#include "Common/DeviceResources.h"
#include "Common/DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace DX;
TextDisplay::TextDisplay(const std::shared_ptr<DX::DeviceResources>& deviceResources, std::wstring text, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlign, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STRETCH fontStretch, float fontSize, D2D1_COLOR_F brushColor, DWRITE_TEXT_ALIGNMENT textAlign, float offsetLeft, float offsetTop, float textWidth, float textHeight) : 
	m_deviceResources(deviceResources), m_text(text), m_paragraphAlign(paragraphAlign), m_fontStyle(fontStyle), m_fontWeight(fontWeight), m_fontStretch(fontStretch), m_fontSize(fontSize), m_brushColor(brushColor), m_textAlign(textAlign), m_offsetLeft(offsetLeft), m_offsetTop(offsetTop), m_textWidth(textWidth), m_textHeight(textHeight){
	ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

	// Create device independent resources
	ComPtr<IDWriteTextFormat> textFormat;
	DX::ThrowIfFailed(
		m_deviceResources->GetDWriteFactory()->CreateTextFormat(
			L"Segoe UI",
			nullptr,
			m_fontWeight,
			m_fontStyle,
			m_fontStretch,
			m_fontSize,
			L"en-US",
			&textFormat
		)
	);

	DX::ThrowIfFailed(
		textFormat.As(&m_textFormat)
	);

	DX::ThrowIfFailed(
		m_textFormat->SetParagraphAlignment(m_paragraphAlign)
	);

	DX::ThrowIfFailed(
		m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
	);

	CreateDeviceDependentResources();
}

void TextDisplay::Update() {
	ComPtr<IDWriteTextLayout> textLayout;
	DX::ThrowIfFailed(
		m_deviceResources->GetDWriteFactory()->CreateTextLayout(
			m_text.c_str(),
			(uint32)m_text.length(),
			m_textFormat.Get(),
			m_textWidth,
			m_textHeight,
			&textLayout
		)
	);

	DX::ThrowIfFailed(
		textLayout.As(&m_textLayout)
	);

	DX::ThrowIfFailed(
		m_textLayout->GetMetrics(&m_textMetrics)
	);
}

void TextDisplay::Render() {
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();

	D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
		m_offsetLeft,
		m_offsetTop
	);

	context->SetTransform(screenTranslation * m_deviceResources->GetOrientationTransform2D());

	DX::ThrowIfFailed(
		m_textFormat->SetTextAlignment(m_textAlign)
	);

	context->DrawTextLayout(
		D2D1::Point2F(0.f, 0.f),
		m_textLayout.Get(),
		m_textBrush.Get()
	);

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

void TextDisplay::CreateDeviceDependentResources() {
	ThrowIfFailed(m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(m_brushColor, &m_textBrush));
}

void TextDisplay::ReleaseDeviceDependentResources() {
	m_textBrush.Reset();
}