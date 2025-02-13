#include "pch.h"
#include "ColorRectangleDisplay.h"
#include "Common/DeviceResources.h"
#include "Common/DirectXHelper.h"
#include "d2d1.h"
#include <wrl/client.h>

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
ColorRectangleDisplay::ColorRectangleDisplay(const std::shared_ptr<DX::DeviceResources>& deviceResources, D2D1_COLOR_F rectangleColor, float offsetLeft, float offsetTop, float width, float height) : 
	m_deviceResources(deviceResources), m_rectColor(rectangleColor), m_offsetLeft(offsetLeft), m_offsetTop(offsetTop), m_width(width), m_height(height) {
	DX::ThrowIfFailed(m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock));
	CreateDeviceDependentResources();
}

void ColorRectangleDisplay::Render() {
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();
	D2D1_RECT_F screenRect = D2D1::RectF(m_offsetLeft, m_offsetTop, m_offsetLeft + m_width, m_offsetTop - m_height);
	context->FillRectangle(&screenRect, m_rectBrush.Get());

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

void ColorRectangleDisplay::CreateDeviceDependentResources() {
	DX::ThrowIfFailed(m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(m_rectColor, &m_rectBrush));
}

void ColorRectangleDisplay::ReleaseDeviceDependentResources() {
	m_rectBrush.Reset();
}