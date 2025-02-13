#pragma once
#include "Common/DeviceResources.h"
#include "Common/DirectXHelper.h"
#include "d2d1.h"
#include <wrl/client.h>

using namespace Microsoft::WRL;
class ColorRectangleDisplay
{
public:
	ColorRectangleDisplay(const std::shared_ptr<DX::DeviceResources>& deviceResources, D2D1_COLOR_F rectangleColor, float offsetLeft, float offsetTop, float width, float height);
	void Render();
	void CreateDeviceDependentResources();
	void ReleaseDeviceDependentResources();
private:
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	D2D1_COLOR_F m_rectColor;
	ComPtr<ID2D1SolidColorBrush> m_rectBrush;
	ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;
	float m_offsetLeft;
	float m_offsetTop;
	float m_width;
	float m_height;
};

