#pragma once

#include <string>
#include "Common\DeviceResources.h"
#include "Common\StepTimer.h"
#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace blenderUWP
{
	// Renders the current FPS value in the bottom right corner of the screen using Direct2D and DirectWrite.
	class MainMenuRenderer
	{
	public:
		MainMenuRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Resources related to text rendering.
		std::wstring                                    m_text;
		DWRITE_TEXT_METRICS	                            m_textMetrics;
		ComPtr<ID2D1SolidColorBrush>    m_brush;
		ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;
		ComPtr<IDWriteTextLayout3>      m_textLayout;
		ComPtr<IDWriteTextFormat2>      m_textFormat;
	};
}