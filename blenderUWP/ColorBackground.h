#pragma once
#include "Common/DeviceResources.h"
#include "Common/DirectXHelper.h"
#include "d2d1.h"
#include <wrl/client.h>

using namespace Microsoft::WRL;
class ColorBackground
{
public:
	ColorBackground(const std::shared_ptr<DX::DeviceResources>& deviceResources, D2D1_COLOR_F backgroundColor);
	void Render();
	void CreateDeviceDependentResources();
	void ReleaseDeviceDependentResources();
private:
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	D2D1_COLOR_F m_backgroundColor;
	ComPtr<ID2D1SolidColorBrush> m_backgroundBrush;
	ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;
};

