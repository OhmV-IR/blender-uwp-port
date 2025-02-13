#include "pch.h"
#include "Common/DeviceResources.h"
#include "Common/DirectXHelper.h"
#include "d2d1.h"
#include <wrl/client.h>
#include "ColorBackground.h"

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
ColorBackground::ColorBackground(const std::shared_ptr<DX::DeviceResources>& deviceResources, D2D1_COLOR_F backgroundColor) : m_deviceResources(deviceResources), m_backgroundColor(backgroundColor) {
	DX::ThrowIfFailed(m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock));
	CreateDeviceDependentResources();
}

void ColorBackground::Render() {
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();
	Size screenSize = m_deviceResources->GetLogicalSize();
	D2D1_RECT_F screenRect = D2D1::RectF(0, screenSize.Height, screenSize.Width, 0);
	context->FillRectangle(&screenRect, m_backgroundBrush.Get());

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

void ColorBackground::CreateDeviceDependentResources() {
	DX::ThrowIfFailed(m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(m_backgroundColor, &m_backgroundBrush));
}

void ColorBackground::ReleaseDeviceDependentResources() {
	m_backgroundBrush.Reset();
}